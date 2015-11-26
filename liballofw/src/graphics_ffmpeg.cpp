#include <iostream>

#include "allofw/graphics.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/glew.h>
# include <GL/gl.h>
#endif


namespace ALLOFW_NS {

namespace {

    void ffmpeg_initialize() {
        static bool ffmpeg_initialized = false;
        if(!ffmpeg_initialized) {
            av_register_all();
            ffmpeg_initialized = true;
        }
    }

    class ByteStreamIOContext {
    private:
        ByteStreamIOContext(const ByteStreamIOContext  &);
        ByteStreamIOContext& operator = (const ByteStreamIOContext &);
    public:
        ByteStreamIOContext(ByteStream* stream)
          : stream_(stream),
            buffer_size_(1024 * 1024),
            buffer_(static_cast<unsigned char*>(av_malloc(buffer_size_)))
        {
            ctx_ = avio_alloc_context(buffer_, buffer_size_, 0, this, &ByteStreamIOContext::read, NULL, &ByteStreamIOContext::seek);
        }

        ~ByteStreamIOContext() {
            av_free(ctx_);
            av_free(buffer_);
        }

        static int read(void *opaque, unsigned char *buf, int buf_size) {
            ByteStreamIOContext* h = static_cast<ByteStreamIOContext*>(opaque);
            return h->stream_->read(buf, buf_size);
        }

        static int64_t seek(void *opaque, int64_t offset, int whence) {
            ByteStreamIOContext* h = static_cast<ByteStreamIOContext*>(opaque);
            if(whence == SEEK_SET) {
                h->stream_->seek(ByteStream::BEGIN, offset);
                return h->stream_->position();
            }
            if(whence == SEEK_CUR) {
                h->stream_->seek(ByteStream::CURRENT, offset);
                return h->stream_->position();
            }
            if(whence == SEEK_END) {
                h->stream_->seek(ByteStream::END, offset);
                return h->stream_->position();
            }
            if(whence == AVSEEK_SIZE) {
                return h->stream_->position();
            }
            return -1;
        }

        AVIOContext *get_avio() { return ctx_; }

    private:
        ByteStream* stream_;
        int buffer_size_;
        unsigned char * buffer_;
        AVIOContext * ctx_;
    };

    class VideoSurface2D_ffmpeg : public VideoSurface2D {
    public:

        VideoSurface2D_ffmpeg(const char* path) {
            std::string error_info;

            ffmpeg_initialize();

            io_context_ = NULL;
            format_context_ = NULL;
            codec_context_ = NULL;
            codec_context_opened_ = false;
            codec_ = NULL;
            frame_ = NULL;
            frame_rgb_ = NULL;
            options_dict_ = NULL;
            buffer_ = NULL;

            //io_context_ = new ByteStreamIOContext(stream);
            //format_context_ = avformat_alloc_context();
            //format_context_->pb = io_context_->get_avio();
            //avformat_open_input(&format_context_, "dummy", nullptr, nullptr);

            if(avformat_open_input(&format_context_, path, NULL, NULL) != 0) {
                _cleanup();
                throw invalid_argument("could not open input file");
            }

            if(avformat_find_stream_info(format_context_, NULL) < 0) {
                _cleanup();
                throw invalid_argument("could not find stream information");
            }

            // av_dump_format(format_context_, 0, "VideoSurface2D_ffmpeg(ByteStream*)", 0);

            video_stream_ = -1;
            for(int i = 0; i < format_context_->nb_streams; i++) {
                if(format_context_->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                    video_stream_ = i;
                    break;
                }
            }

            if(video_stream_ == -1) {
                _cleanup();
                throw invalid_argument("could not find video stream");
            }

            codec_context_ = format_context_->streams[video_stream_]->codec;

            codec_ = avcodec_find_decoder(codec_context_->codec_id);

            if(codec_ == NULL) {
                _cleanup();
                throw invalid_argument("could not find video decoder");
            }

            if(avcodec_open2(codec_context_, codec_, &options_dict_) < 0) {
                _cleanup();
                throw invalid_argument("could not open codec");
            }
            codec_context_opened_ = true;

            frame_ = av_frame_alloc();
            frame_rgb_ = av_frame_alloc();

            int num_bytes_ = avpicture_get_size(PIX_FMT_RGBA, codec_context_->width, codec_context_->height);
            buffer_ = (uint8_t *)av_malloc(num_bytes_ * sizeof(uint8_t));

            sws_ctx = sws_getContext(
                codec_context_->width,
                codec_context_->height,
                codec_context_->pix_fmt,
                codec_context_->width,
                codec_context_->height,
                PIX_FMT_RGBA, SWS_FAST_BILINEAR,
                NULL, NULL, NULL
            );

            time_base_ = (int64_t(codec_context_->time_base.num) * AV_TIME_BASE) / int64_t(codec_context_->time_base.den);
            fps_ = (double)format_context_->streams[video_stream_]->r_frame_rate.num / (double)format_context_->streams[video_stream_]->r_frame_rate.den;
            duration_ = (double)format_context_->duration / (double)AV_TIME_BASE;

            // Assign appropriate parts of buffer to image planes in pFrameRGB
            // Note that pFrameRGB is an AVFrame, but AVFrame is a superset of AVPicture
            avpicture_fill((AVPicture*)frame_rgb_, buffer_, PIX_FMT_RGBA, codec_context_->width, codec_context_->height);
        }

        void _cleanup() {
            if(codec_context_opened_) { avcodec_close(codec_context_); }
            if(frame_) { av_free(frame_); frame_ = NULL; }
            if(frame_rgb_) { av_free(frame_rgb_); frame_rgb_ = NULL; }
            if(format_context_) { avformat_close_input(&format_context_); format_context_ = NULL; }
            if(io_context_) { delete io_context_; io_context_ = NULL; }
            if(buffer_) { av_free(buffer_); }
        }

        virtual ~VideoSurface2D_ffmpeg() {
            _cleanup();
        }

        virtual int width() const {
            return codec_context_->width;
        }

        virtual int height() const {
            return codec_context_->height;
        }

        virtual void seek(double time) {
            av_seek_frame(format_context_, -1, int64_t(time * AV_TIME_BASE), AVSEEK_FLAG_ANY);
            avcodec_flush_buffers(codec_context_);
        }

        virtual bool nextFrame() {
            while(av_read_frame(format_context_, &packet_) >= 0) {
                if(packet_.stream_index == video_stream_) {
                    int frame_finished;
                    avcodec_decode_video2(codec_context_, frame_, &frame_finished, &packet_);
                    if(frame_finished) {
                        sws_scale(
                            sws_ctx,
                            (uint8_t const * const *)frame_->data,
                            frame_->linesize,
                            0,
                            codec_context_->height,
                            frame_rgb_->data,
                            frame_rgb_->linesize
                        );
                        return true;
                    }
                }
            }
            return false;
        }

        virtual double fps() {
            return fps_;
        }

        virtual double duration() {
            return duration_;
        }

        // Let the class write to this pixel buffer.
        virtual void setPixelBuffer(void* buffer) {
            assigned_buffer_ = (uint8_t*)buffer;
            if(assigned_buffer_) {
                avpicture_fill((AVPicture*)frame_rgb_, assigned_buffer_, PIX_FMT_RGBA, codec_context_->width, codec_context_->height);
            } else {
                avpicture_fill((AVPicture*)frame_rgb_, buffer_, PIX_FMT_RGBA, codec_context_->width, codec_context_->height);
            }
        }

        virtual const void* pixels() const {
            return buffer_;
        }

        ByteStreamIOContext  *io_context_;
        AVFormatContext      *format_context_;
        AVCodecContext       *codec_context_;
        bool                  codec_context_opened_;
        AVCodec              *codec_;
        AVFrame              *frame_;
        AVFrame              *frame_rgb_;
        AVDictionary         *options_dict_;
        AVPacket              packet_;
        int                   video_stream_;
        int                   frameFinished;
        int                   num_bytes_;
        uint8_t              *buffer_;
        uint8_t              *assigned_buffer_;
        int64_t               time_base_;
        double                fps_;
        double                duration_;
        struct SwsContext    *sws_ctx;
    };

}

VideoSurface2D* FFMPEG_VideoSurface2D_FromFile(const char* path) {
    return new VideoSurface2D_ffmpeg(path);
}

void FFMPEG_VideoSurface2D_Destroy(VideoSurface2D* surface) {
    delete (VideoSurface2D_ffmpeg*)surface;
}

}
