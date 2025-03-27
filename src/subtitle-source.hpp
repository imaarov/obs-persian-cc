#ifndef SUBTITLE_SOURCE_H
#define SUBTITLE_SOURCE_H

#include <obs-module.h>
#include <whisper.h>
#include <string>

class SubtitleSource {
public:
    SubtitleSource(obs_data_t* settings, obs_source_t* source);
    ~SubtitleSource();

    void update(obs_data_t* settings);
    void render(gs_effect_t* effect);
    static void audioCaptureCallback(void* data, obs_source_t* source, const struct audio_data* audio, bool muted);
    pcmf32_buffer_
private:
    obs_source_t* source_;
    whisper_context* whisper_ctx_;
    std::string subtitle_text_;
    float* pcmf32_buffer_;
    size_t pcmf32_size_;
    obs_source_t* audio_source_;
};

#endif