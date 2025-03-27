#include "subtitle-source.hpp"
#include <vector>

SubtitleSource::SubtitleSource(obs_data_t* settings, obs_source_t* source): 
    source_(source),
    whisper_ctx_(nullptr), 
    subtitle_text_(""), 
    pcmf32_buffer_(nullptr), 
    pcmf32_size_(0), 
    audio_source_(nullptr) {
    struct whisper_context_params params = whisper_context_default_params();
    whisper_ctx_ = whisper_init_from_file_with_params("data/obs-plugins/persian-subtitle-plugin/models/base.bin", params);
    if (!whisper_ctx_) {
        blog(LOG_ERROR, "Failed to initialize Whisper context, the model not founded");
    }

    const char* audio_source_name = obs_data_get_string(settings, "audio_source");
    if (audio_source_name && *audio_source_name) {
        audio_source_ = obs_get_source_by_name(audio_source_name);
        if (audio_source_) {
            obs_source_add_audio_capture_callback(audio_source_, audioCaptureCallback, this);
        }
    }
}

SubtitleSource::~SubtitleSource() {
    if (audio_source_) {
        obs_source_remove_audio_capture_callback(audio_source_, audioCaptureCallback, this);
        obs_source_release(audio_source_);
    }
    if (whisper_ctx_) {
        whisper_free(whisper_ctx_);
    }
    if (pcmf32_buffer_) {
        free(pcmf32_buffer_);
    }
}

void SubtitleSource::update(obs_data_t* settings) {
    const char* audio_source_name = obs_data_get_string(settings, "audio_source");
    if (audio_source_name && *audio_source_name) {
        obs_source_t* new_audio_source = obs_get_source_by_name(audio_source_name);
        if (new_audio_source != audio_source_) {
            if (audio_source_) {
                obs_source_remove_audio_capture_callback(audio_source_, audioCaptureCallback, this);
                obs_source_release(audio_source_);
            }
            audio_source_ = new_audio_source;
            if (audio_source_) {
                obs_source_add_audio_capture_callback(audio_source_, audioCaptureCallback, this);
            }
        }
    }
}

void SubtitleSource::audioCaptureCallback(void* data, obs_source_t* source, const struct audio_data* audio, bool muted) {
    SubtitleSource* s = static_cast<SubtitleSource*>(data);
    if (!s || !s->whisper_ctx_ || muted || !audio->frames) {
        return;
    }

    const uint8_t* input = audio->data[0];
    size_t sample_count = audio->frames;
    if (s->pcmf32_size_ < sample_count) {
        if (s->pcmf32_buffer_) {
            free(s->pcmf32_buffer_);
        }
        s->pcmf32_buffer_ = (float*)malloc(sample_count * sizeof(float));
        s->pcmf32_size_ = sample_count;
    }

    const int16_t* pcm16 = reinterpret_cast<const int16_t*>(input);
    for (size_t i = 0; i < sample_count; i++) {
        s->pcmf32_buffer_[i] = pcm16[i] / 32768.0f;
    }

    struct whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    wparams.n_threads = 4;
    wparams.print_realtime = false;
    wparams.print_progress = false;
    wparams.print_timestamps = false;

    if (whisper_full(s->whisper_ctx_, wparams, s->pcmf32_buffer_, sample_count) == 0) {
        int n_segments = whisper_full_n_segments(s->whisper_ctx_);
        if (n_segments > 0) {
            s->subtitle_text_ = whisper_full_get_segment_text(s->whisper_ctx_, n_segments - 1);
            blog(LOG_INFO, "Subtitle: %s", s->subtitle_text_.c_str());
        }
    }
}

void SubtitleSource::render(gs_effect_t* effect) {
    if (subtitle_text_.empty()) {
        return;
    }

    gs_effect_t* solid = obs_get_base_effect(OBS_EFFECT_SOLID);
    gs_eparam_t* color_param = gs_effect_get_param_by_name(solid, "color");
    gs_effect_set_color(color_param, 0xFFFFFFFF);

    gs_matrix_push();
    gs_matrix_identity();
    gs_matrix_translate3f(50.0f, 50.0f, 0.0f);

    blog(LOG_INFO, "Rendering subtitle: %s", subtitle_text_.c_str());

    gs_matrix_pop();
}