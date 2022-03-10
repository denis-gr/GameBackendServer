#pragma once

#include <string>

#include <userver/components/component_fwd.hpp>
#include <userver/components/loggable_component_base.hpp>

USERVER_NAMESPACE_BEGIN

namespace components {

// clang-format off

/// @ingroup userver_components
///
/// @brief Helper component that manages common configuration for userver dumps.
///
/// The component must be configured in service config.
///
/// ## Static options:
/// Name | Description | Default value
/// ---- | ----------- | -------------
/// dump-root | Components store dumps in subdirectories of this directory | -
///
/// ## Config example:
///
/// @snippet components/common_component_list_test.cpp Sample dump configurator component config

// clang-format on
class DumpConfigurator final : public LoggableComponentBase {
 public:
  static constexpr auto kName = "dump-configurator";

  DumpConfigurator(const ComponentConfig& config,
                   const ComponentContext& context);

  const std::string& GetDumpRoot() const;

  static std::string GetStaticConfigSchema();

 private:
  const std::string dump_root_;
};

template <>
inline constexpr bool kHasValidate<DumpConfigurator> = true;

}  // namespace components

USERVER_NAMESPACE_END
