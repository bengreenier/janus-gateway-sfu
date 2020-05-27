#include <plugin.h>

/* Plugin information */
#define SFU_VERSION 8
#define SFU_VERSION_STRING "0.0.8"
#define SFU_DESCRIPTION "A Barebones RTP SFU for Janus Gateway."
#define SFU_NAME "SFU"
#define SFU_AUTHOR "bengreenier"
#define SFU_PACKAGE "com.bengreenier.sfu"

struct janus_plugin_result *sfu_handle_message(janus_plugin_session *handle,
                                               char *transaction,
                                               json_t *message, json_t *jsep);

janus_plugin *create(void);
int sfu_init(janus_callbacks *callback, const char *config_path);
void sfu_destroy(void);
int sfu_get_api_compatibility(void);
int sfu_get_version(void);
const char *sfu_get_version_string(void);
const char *sfu_get_description(void);
const char *sfu_get_name(void);
const char *sfu_get_author(void);
const char *sfu_get_package(void);
void sfu_create_session(janus_plugin_session *handle, int *error);
json_t *sfu_handle_admin_message(json_t *message);
void sfu_setup_media(janus_plugin_session *handle);
void sfu_incoming_rtp(janus_plugin_session *handle, janus_plugin_rtp *packet);
void sfu_incoming_rtcp(janus_plugin_session *handle, janus_plugin_rtcp *packet);
void sfu_data_ready(janus_plugin_session *handle);
void sfu_hangup_media(janus_plugin_session *handle);
void sfu_destroy_session(janus_plugin_session *handle, int *error);
json_t *sfu_query_session(janus_plugin_session *handle);

/* Plugin setup */
static janus_plugin sfu_plugin =
    JANUS_PLUGIN_INIT(.init = sfu_init, .destroy = sfu_destroy,

                      .get_api_compatibility = sfu_get_api_compatibility,
                      .get_version = sfu_get_version,
                      .get_version_string = sfu_get_version_string,
                      .get_description = sfu_get_description,
                      .get_name = sfu_get_name, .get_author = sfu_get_author,
                      .get_package = sfu_get_package,

                      .create_session = sfu_create_session,
                      .handle_message = sfu_handle_message,
                      .handle_admin_message = sfu_handle_admin_message,
                      .setup_media = sfu_setup_media,
                      .incoming_rtp = sfu_incoming_rtp,
                      .incoming_rtcp = sfu_incoming_rtcp,
                      .data_ready = sfu_data_ready,
                      .hangup_media = sfu_hangup_media,
                      .destroy_session = sfu_destroy_session,
                      .query_session = sfu_query_session);

janus_plugin *create(void) { return &sfu_plugin; }

int sfu_get_api_compatibility(void) { return JANUS_PLUGIN_API_VERSION; }
int sfu_get_version(void) { return SFU_VERSION; }
const char *sfu_get_version_string(void) { return SFU_VERSION_STRING; }
const char *sfu_get_description(void) { return SFU_DESCRIPTION; }
const char *sfu_get_name(void) { return SFU_NAME; }
const char *sfu_get_author(void) { return SFU_AUTHOR; }
const char *sfu_get_package(void) { return SFU_PACKAGE; }

// the remaining functions are glue-ed to the cpp layer
// you may view them in glue.cpp
