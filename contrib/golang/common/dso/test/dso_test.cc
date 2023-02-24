#include <string>

#include "envoy/registry/registry.h"

#include "test/test_common/environment.h"
#include "test/test_common/utility.h"

#include "contrib/golang/common/dso/dso.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Dso {
namespace {

std::string genSoPath(std::string name) {
  return TestEnvironment::substitute("{{ test_rundir }}/contrib/golang/common/dso/test/test_data/" +
                                     name);
}

TEST(DsoInstanceTest, SimpleAPI) {
  auto path = genSoPath("simple.so");
  HttpFilterDsoInstancePtr dso(new HttpFilterDsoInstance(path));
  EXPECT_EQ(dso->envoyGoFilterNewHttpPluginConfig(0, 0), 100);
}

TEST(DsoInstanceManagerTest, Pub) {
  auto id = "simple.so";
  auto path = genSoPath(id);

  // get before load http filter dso
  auto dso = DsoInstanceManager<HttpFilterDsoInstance>::getDsoInstanceByID(id);
  EXPECT_EQ(dso, nullptr);

  // first time load http filter dso
  auto res = DsoInstanceManager<HttpFilterDsoInstance>::load(id, path);
  EXPECT_EQ(res, true);

  // get after load http filter dso
  dso = DsoInstanceManager<HttpFilterDsoInstance>::getDsoInstanceByID(id);
  EXPECT_NE(dso, nullptr);
  EXPECT_EQ(dso->envoyGoFilterNewHttpPluginConfig(0, 0), 100);

  // second time load http filter dso
  res = DsoInstanceManager<HttpFilterDsoInstance>::load(id, path);
  EXPECT_EQ(res, true);

  // first time load cluster specifier dso
  res = DsoInstanceManager<ClusterSpecifierDsoInstance>::load(id, path);
  EXPECT_EQ(res, true);

  // get after load cluster specifier dso
  auto cluster_dso = DsoInstanceManager<ClusterSpecifierDsoInstance>::getDsoInstanceByID(id);
  EXPECT_NE(cluster_dso, nullptr);

  EXPECT_EQ(cluster_dso->envoyGoClusterSpecifierNewPlugin(0, 0), 200);
}

} // namespace
} // namespace Dso
} // namespace Envoy
