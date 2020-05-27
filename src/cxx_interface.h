#include <jansson.h>

#include <string>

namespace sfu {
struct RtpExtensions {
  /*! \brief Audio level, in DB (0-127, 127=silence) = 0; -1 means no extension
   */
  int8_t audio_level;
  /*! \brief Whether the encoder detected voice activity (part of audio-level
   * extension)
   * @note Browsers apparently always set this to 1, so it's unreliable and
   * should be ignored */
  bool audio_level_vad;
  /*! \brief Video orientation rotation (0, 90, 180, 270) = 0; -1 means no
   * extension
   */
  int16_t video_rotation;
  /*! \brief Whether the video orientation extension says this is the back
   * camera
   * @note Will be ignored if no rotation value is set */
  bool video_back_camera;
  /*! \brief Whether the video orientation extension says it's flipped
   * horizontally
   * @note Will be ignored if no rotation value is set */
  bool video_flipped;
};

struct Rtp {
  /*! \brief Whether this is an audio or video RTP packet */
  bool video;
  /*! \brief The packet data */
  std::string buffer;
  /*! \brief RTP extensions */
  RtpExtensions extensions;
};

struct Rtcp {
  /*! \brief Whether this is an audio or video RTCP packet */
  bool video;
  /*! \brief The packet data */
  std::string buffer;
};

struct SessionHandle {
  /*! \brief The raw janus c pointer to the session */
  void *raw;
};

struct PluginData {
  /*! \brief The label this message belongs to */
  std::string label;
  /*! \brief The subprotocol this message refers to */
  std::string protocol;
  /*! \brief Whether the message data is text (default=FALSE) or binary */
  bool binary;
  /*! \brief The message data */
  std::string buffer;
};

struct CoreCommunicationProvider {
  virtual json_t HandleAdmin(json_t msg) = 0;

  /*! \brief Callback to push events/messages to a peer
   * @note The Janus core increases the references to both the message and jsep
   * json_t objects. This means that you'll have to decrease your own
   * reference yourself with a \c json_decref after calling push_event.
   * @param[in] handle The plugin/gateway session used for this peer
   * @param[in] transaction The transaction identifier this message refers to
   * @param[in] message The json_t object containing the JSON message
   * @param[in] jsep The json_t object containing the JSEP type, the SDP
   * attached to the message/event, if any (offer/answer), and whether this is
   * an update */
  virtual int push_event(SessionHandle handle, std::string transaction,
                         json_t message, json_t jsep) = 0;

  /*! \brief Callback to relay RTP packets to a peer
   * @param[in] handle The plugin/gateway session used for this peer
   * @param[in] packet The RTP packet and related data */
  virtual void relay_rtp(SessionHandle handle, Rtp packet) = 0;

  /*! \brief Callback to relay RTCP messages to a peer
   * @param[in] handle The plugin/gateway session that will be used for this
   * peer
   * @param[in] packet The RTCP packet and related data */
  virtual void relay_rtcp(SessionHandle handle, Rtcp packet) = 0;

  /*! \brief Callback to relay SCTP/DataChannel messages to a peer
   * @note The protocol is only used for the first message sent on a new data
   * channel, as it will be used to create it; it will be ignored for following
   * messages on the same label, so you can set NULL after that
   * @param[in] handle The plugin/gateway session that will be used for this
   * peer
   * @param[in] packet The message data and related info */
  virtual void relay_data(SessionHandle handle, PluginData packet) = 0;

  /*! \brief Helper to ask for a keyframe via a RTCP PLI
   * @note This is a shortcut, as it is also possible to do the same by crafting
   * an RTCP PLI message manually, and passing it to the core via relay_rtcp
   * @param[in] handle The plugin/gateway session that will be used for this
   * peer */
  virtual void send_pli(SessionHandle handle) = 0;

  /*! \brief Helper to ask for a keyframe via a RTCP PLI
   * @note This is a shortcut, as it is also possible to do the same by crafting
   * an RTCP REMB message manually, and passing it to the core via relay_rtcp
   * @param[in] handle The plugin/gateway session that will be used for this
   * peer
   * @param[in] bitrate The bitrate value to send in the REMB message */
  virtual void send_remb(SessionHandle handle, uint32_t bitrate) = 0;

  /*! \brief Callback to ask the core to close a WebRTC PeerConnection
   * \note A call to this method will result in the core invoking the
   * hangup_media callback on this plugin when done
   * @param[in] handle The plugin/gateway session that the PeerConnection is
   * related to */
  virtual void close_pc(SessionHandle handle) = 0;

  /*! \brief Callback to ask the core to get rid of a plugin/gateway session
   * \note A call to this method will result in the core invoking the
   * destroy_session callback on this plugin when done
   * @param[in] handle The plugin/gateway session to get rid of */
  virtual void end_session(SessionHandle handle) = 0;

  /*! \brief Callback to check whether the event handlers mechanism is enabled
   * @returns TRUE if it is, FALSE if it isn't (which means notify_event should
   * NOT be called) */
  virtual bool events_is_enabled(void) = 0;

  /*! \brief Callback to notify an event to the registered and subscribed event
   * handlers \note Don't unref the event object, the core will do that for you
   * @param[in] handle The plugin/gateway session originating the event, if any
   * @param[in] event The event to notify as a Jansson json_t object */
  virtual void notify_event(SessionHandle handle, json_t event) = 0;

  /*! \brief Method to check whether a signed token is valid
   * \note accepts only tokens with the plugin identifier as realm
   * @param[in] token The token to validate
   * @returns TRUE if the signature is valid and not expired, FALSE otherwise */
  virtual bool auth_is_signature_valid(std::string token) = 0;

  /*! \brief Method to verify a signed token grants access to a descriptor
   * \note accepts only tokens with the plugin identifier as realm
   * @param[in] token The token to validate
   * @param[in] desc The descriptor to search for
   * @returns TRUE if the token is valid, not expired and contains the
   * descriptor, FALSE otherwise */
  virtual bool auth_signature_contains(std::string token,
                                       std::string descriptor) = 0;
};
}  // namespace sfu
