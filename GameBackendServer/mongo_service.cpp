#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/formats/bson/inline.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/formats/json.hpp>
#include <userver/crypto/hash.hpp>
#include <string>

#include <userver/utest/using_namespace_userver.hpp>

namespace GameBackendServer {


class UserRegistrationHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-registration";
  storages::mongo::PoolPtr pool_;

  UserRegistrationHandler(const components::ComponentConfig& config,
                          const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pool_(context.FindComponent<components::Mongo>("mongo-tr").GetPool()) {}

  std::string HandleRequestThrow(
    const server::http::HttpRequest& request,
    server::request::RequestContext&) const override {
      formats::json::Value json = formats::json::FromString(request.RequestBody());
      const std::string username = json["username"].As<std::string>();
      const std::string password = json["password"].As<std::string>();
      auto users = pool_->GetCollection("users");
      using formats::bson::MakeDoc;
      size_t count_user = users.Count(MakeDoc("username", username));
      formats::json::ValueBuilder vb;
      if (count_user != 0) {
        vb["detail"] = "User with this username already exists";
        request.SetResponseStatus(server::http::HttpStatus::kConflict);
      } else {
        const std::string hashed_password = crypto::hash::Blake2b128(password);
        users.InsertOne(MakeDoc("username", username,"hashed_password", hashed_password));
        formats::json::ValueBuilder vb;
        vb["username"] = username;
        request.SetResponseStatus(server::http::HttpStatus::kCreated);     
      };
      return ToString(vb.ExtractValue());
    };
};


/*

class UserLoginHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-login";
  storages::mongo::PoolPtr pool_;

  UserLoginHandler(const components::ComponentConfig& config,
                  const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pool_(context.FindComponent<components::Mongo>("mongo-tr").GetPool()) {}

  std::string HandleRequestThrow(
    const server::http::HttpRequest& request,
    server::request::RequestContext&) const override {
      formats::json::Value json = formats::json::FromString(request.RequestBody());
      const std::string username = json["username"].As<std::string>();
      const std::string password = json["password"].As<std::string>();
      auto users = pool_->GetCollection("users");
      using formats::bson::MakeDoc;
      size_t count_user = users.Find(MakeDoc("username", username));
      formats::json::ValueBuilder vb;
      if (count_user == 0) {
        vb["detail"] = "Incorrect username or password";
        request.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
      } else {
        auto user = users.Find(MakeDoc("username", username));
        const std::string hashed_true_password = crypto::hash::Blake2b128(password);
        const std::string hashed_user_password = crypto::hash::Blake2b128(password);
        if (hashed_true_password != hashed_user_password) {
          vb["detail"] = "Incorrect username or password";
          request.SetResponseStatus(server::http::HttpStatus::kUnauthorized)
        } else {
            //TODO
        };

        formats::json::ValueBuilder vb;
        vb["access_token"] = access_token;
        vb["token_type"] = token_type;
        request.SetResponseStatus(server::http::HttpStatus::kOk);     
      };
      return ToString(vb.ExtractValue());
    };
};


class UserMeHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-me";
  storages::mongo::PoolPtr pool_;

  UserMeHandler(const components::ComponentConfig& config,
                  const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pool_(context.FindComponent<components::Mongo>("mongo-tr").GetPool()) {}

  std::string HandleRequestThrow(
    const server::http::HttpRequest& request,
    server::request::RequestContext&) const override {};
  private:

};


class UserOtherHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-other";
  storages::mongo::PoolPtr pool_;

  UserOtherHandler(const components::ComponentConfig& config,
                  const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      pool_(context.FindComponent<components::Mongo>("mongo-tr").GetPool()) {}

  std::string HandleRequestThrow(
    const server::http::HttpRequest& request,
    server::request::RequestContext&) const override {
      formats::json::Value json = formats::json::FromString(request.RequestBody());
      const std::string username = json["username"].As<std::string>();
      const std::string password = json["password"].As<std::string>();
      auto users = pool_->GetCollection("users");
      using formats::bson::MakeDoc;
      size_t count_user = users.Find(MakeDoc("username", username));
      formats::json::ValueBuilder vb;
      if (count_user == 0) {
        vb["detail"] = "Incorrect username or password";
        request.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
      } else {
        auto user = users.Find(MakeDoc("username", username));
        const std::string hashed_true_password = crypto::hash::Blake2b128(password);
        const std::string hashed_user_password = crypto::hash::Blake2b128(password);
        if (hashed_true_password != hashed_user_password) {
          vb["detail"] = "Incorrect username or password";
          request.SetResponseStatus(server::http::HttpStatus::kUnauthorized)
        } else {
            //TODO
        };

        formats::json::ValueBuilder vb;
        vb["access_token"] = access_token;
        vb["token_type"] = token_type;
        request.SetResponseStatus(server::http::HttpStatus::kOk);     
      };
      return ToString(vb.ExtractValue());
    };
};








*/






}












/*

/// [Mongo service sample - component]
namespace samples::mongodb {



class Translations final : public server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-translations";

  Translations(const components::ComponentConfig& config,
               const components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pool_(context.FindComponent<components::Mongo>("mongo-tr").GetPool()) {}

  std::string HandleRequestThrow(
      const server::http::HttpRequest& request,
      server::request::RequestContext&) const override {
    if (request.GetMethod() == server::http::HttpMethod::kPatch) {
      InsertNew(request);
      return {};
    } else {
      return ReturnDiff(request);
    }
  }

 private:
  void InsertNew(const server::http::HttpRequest& request) const;
  std::string ReturnDiff(const server::http::HttpRequest& request) const;

  storages::mongo::PoolPtr pool_;
};

}  // namespace samples::mongodb
/// [Mongo service sample - component]

namespace samples::mongodb {

/// [Mongo service sample - InsertNew]
void Translations::InsertNew(const server::http::HttpRequest& request) const {
  const auto& key = request.GetArg("key");
  const auto& lang = request.GetArg("lang");
  const auto& value = request.GetArg("value");

  using formats::bson::MakeDoc;
  auto transl = pool_->GetCollection("translations2");
  transl.InsertOne(MakeDoc("key", key, "lang", lang, "value", value));
  request.SetResponseStatus(server::http::HttpStatus::kCreated);
}
/// [Mongo service sample - InsertNew]

/// [Mongo service sample - ReturnDiff]
std::string Translations::ReturnDiff(
    const server::http::HttpRequest& request) const {
  auto time_point = std::chrono::system_clock::time_point{};
  if (request.HasArg("last_update")) {
    const auto& update_time = request.GetArg("last_update");
    time_point = utils::datetime::Stringtime(update_time);
  }

  using formats::bson::MakeDoc;
  namespace options = storages::mongo::options;
  auto transl = pool_->GetCollection("translations2");
  auto cursor = transl.Find(
      MakeDoc("_id",
              MakeDoc("$gte", formats::bson::Oid::MakeMinimalFor(time_point))),
      options::Sort{std::make_pair("_id", options::Sort::kAscending)});

  if (!cursor) {
    return "{}";
  }

  formats::json::ValueBuilder vb;
  auto content = vb["content"];

  formats::bson::Value last;
  for (const auto& doc : cursor) {
    const auto key = doc["key"].As<std::string>();
    const auto lang = doc["lang"].As<std::string>();
    content[key][lang] = doc["value"].As<std::string>();

    last = doc;
  }

  vb["update_time"] = utils::datetime::Timestring(
      last["_id"].As<formats::bson::Oid>().GetTimePoint());

  return ToString(vb.ExtractValue());
}
/// [Mongo service sample - ReturnDiff]

}  // namespace samples::mongodb
*/

int main(int argc, char* argv[]) {
  const auto component_list = components::MinimalServerComponentList()
                                  .Append<clients::dns::Component>()
                                  .Append<components::Mongo>("mongo-tr")
                                  .Append<GameBackendServer::UserRegistrationHandler>();
  return utils::DaemonMain(argc, argv, component_list);
}
