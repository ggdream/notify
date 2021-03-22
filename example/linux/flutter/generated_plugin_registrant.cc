//
//  Generated file. Do not edit.
//

#include "generated_plugin_registrant.h"

#include <notify/notify_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) notify_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "NotifyPlugin");
  notify_plugin_register_with_registrar(notify_registrar);
}
