#include <jansson.h>

#include <map>
#include <memory>
#include <shared_mutex>

#include "cxx_interface.h"

namespace sfu {
class Session {
 public:
  void SetupMedia();
  void CleanupMedia();

  void HandleRtp(Rtp rtp);
  void HandleRtcp(Rtcp rtcp);

  json_t Query();

  void HandleData();
};

class SessionManager : public CoreCommunicationProvider {
 public:
  std::shared_ptr<Session> CreateSession(SessionHandle handle);
  std::shared_ptr<Session> GetSession(SessionHandle handle);
  bool RemoveSession(SessionHandle handle);

  // CoreCommunication interface

  json_t HandleAdmin(json_t msg) override;

  int push_event(SessionHandle handle, std::string transaction, json_t message,
                 json_t jsep) override;

  void relay_rtp(SessionHandle handle, Rtp packet) override;

  void relay_rtcp(SessionHandle handle, Rtcp packet) override;

  void relay_data(SessionHandle handle, PluginData packet) override;

  void send_pli(SessionHandle handle) override;

  void send_remb(SessionHandle handle, uint32_t bitrate) override;

  void close_pc(SessionHandle handle) override;

  void end_session(SessionHandle handle) override;

  bool events_is_enabled(void) override;

  void notify_event(SessionHandle handle, json_t event) override;

  bool auth_is_signature_valid(std::string token) override;

  bool auth_signature_contains(std::string token,
                               std::string descriptor) override;

 private:
  std::map<SessionHandle, std::shared_ptr<Session>> session_store_;
};

}  // namespace sfu
