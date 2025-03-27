/*
PersianCC
Copyright (C) 2025 imaarov info@imaarov.ir

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/
#include <obs-module.h>
#include "subtitle-source.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("persian-subtitle-plugin", "en-US")

static void* subtitle_source_create(obs_data_t* settings, obs_source_t* source) {
    return new SubtitleSource(settings, source);
}

static void subtitle_source_destroy(void* data) {
    delete static_cast<SubtitleSource*>(data);
}

static void subtitle_source_update(void* data, obs_data_t* settings) {
    static_cast<SubtitleSource*>(data)->update(settings);
}

static void subtitle_source_render(void* data, gs_effect_t* effect) {
    static_cast<SubtitleSource*>(data)->render(effect);
}

static obs_properties_t* subtitle_source_properties(void* data) {
    obs_properties_t* props = obs_properties_create();
    obs_properties_add_text(props, "audio_source", "Audio Source Name", OBS_TEXT_DEFAULT);
    return props;
}

static struct obs_source_info subtitle_source_info = {
    .id = "persian_subtitle_source",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_AUDIO,
    .get_name = [](void*) { return "Persian Subtitles"; },
    .create = subtitle_source_create,
    .destroy = subtitle_source_destroy,
    .get_properties = subtitle_source_properties,
    .update = subtitle_source_update,
    .video_render = subtitle_source_render,
};

bool obs_module_load(void) {
    obs_register_source(&subtitle_source_info);
    return true;
}

void obs_module_unload(void) {}