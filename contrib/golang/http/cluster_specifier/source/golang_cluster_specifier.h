#pragma once

#include "source/common/common/base64.h"
#include "source/common/http/utility.h"

#include "envoy/router/cluster_specifier_plugin.h"

#include "contrib/envoy/extensions/http/cluster_specifier/golang/v3alpha/golang.pb.h"
#include "contrib/golang/common/dso/dso.h"

namespace Envoy {
namespace Router {
namespace Golang {

using GolangClusterProto = envoy::extensions::http::cluster_specifier::golang::v3alpha::Config;

class ClusterConfig : Logger::Loggable<Logger::Id::http> {
public:
  ClusterConfig(const GolangClusterProto& config);
  uint64_t getPluginId() { return plugin_id_; };
  const std::string& defaultCluster() { return default_cluster_; }
  Dso::DsoInstancePtr getDsoLib() { return dynamic_lib_; }

private:
  const std::string so_id_;
  const std::string so_path_;
  const std::string default_cluster_;
  const Protobuf::Any config_;
  uint64_t plugin_id_{0};
  Dso::DsoInstancePtr dynamic_lib_;
};

using ClusterConfigSharedPtr = std::shared_ptr<ClusterConfig>;

class GolangClusterSpecifierPlugin : public ClusterSpecifierPlugin,
                                     Logger::Loggable<Logger::Id::http> {
public:
  GolangClusterSpecifierPlugin(ClusterConfigSharedPtr config) : config_(config){};

  RouteConstSharedPtr route(const RouteEntry& parent, const Http::RequestHeaderMap&) const;
  void log(absl::string_view& msg) const;

private:
  ClusterConfigSharedPtr config_;
};

} // namespace Golang
} // namespace Router
} // namespace Envoy
