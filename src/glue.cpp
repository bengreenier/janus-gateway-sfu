#include <plugin.h>

#include <memory>

#include "sfu.h"

namespace {
// the global session manager
std::unique_ptr<sfu::SessionManager> s_manager_;

// Maps a c handle as an opaque pointer to a cpp handle
sfu::SessionHandle ToHandle(void *hndl) { return sfu::SessionHandle{hndl}; }

sfu::Rtp ToWrapper(janus_plugin_rtp *rtp) {
  sfu::Rtp instance;
  instance.buffer = std::string(rtp->buffer, rtp->length);
  instance.video = rtp->video ? true : false;
  instance.extensions.audio_level = rtp->extensions.audio_level;
  instance.extensions.audio_level_vad = rtp->extensions.audio_level_vad;
  instance.extensions.video_back_camera = rtp->extensions.video_back_camera;
  instance.extensions.video_flipped = rtp->extensions.video_flipped;
  instance.extensions.video_rotation = rtp->extensions.video_rotation;

  return instance;
}

sfu::Rtcp ToWrapper(janus_plugin_rtcp *rtcp) {
  sfu::Rtcp instance;
  instance.video = rtcp->video ? true : false;
  instance.buffer = std::string(rtcp->buffer, rtcp->length);

  return instance;
}
}  // namespace

// glue layer for c => cpp
extern "C" {
int sfu_init(janus_callbacks *callback, const char *config_path) {
  s_manager_ = std::make_unique<sfu::SessionManager>();

  return 0;
}
void sfu_destroy(void) { s_manager_.reset(); }

void sfu_create_session(janus_plugin_session *handle, int *error) {
  s_manager_->CreateSession(ToHandle(handle));
}
void sfu_destroy_session(janus_plugin_session *handle, int *error) {
  s_manager_->RemoveSession(ToHandle(handle));
}

json_t *sfu_handle_admin_message(json_t *message) {
  return &s_manager_->HandleAdmin(*message);
}

void sfu_setup_media(janus_plugin_session *handle) {
  s_manager_->GetSession(ToHandle(handle))->SetupMedia();
}
void sfu_incoming_rtp(janus_plugin_session *handle, janus_plugin_rtp *packet) {
  s_manager_->GetSession(ToHandle(handle))->HandleRtp(ToWrapper(packet));
}
void sfu_incoming_rtcp(janus_plugin_session *handle,
                       janus_plugin_rtcp *packet) {
  s_manager_->GetSession(ToHandle(handle))->HandleRtcp(ToWrapper(packet));
}
void sfu_data_ready(janus_plugin_session *handle) {
  s_manager_->GetSession(ToHandle(handle))->HandleData();
}
void sfu_hangup_media(janus_plugin_session *handle) {
  s_manager_->GetSession(ToHandle(handle))->CleanupMedia();
}

json_t *sfu_query_session(janus_plugin_session *handle) {
  return &s_manager_->GetSession(ToHandle(handle))->Query();
}
}