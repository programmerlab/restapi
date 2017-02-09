<?php

/**
 * Implements hook_drush_command().
 *
 */
function restapi_drush_command() {

  $items['restapi-list'] = [
    'description' => dt('Returns a list of available resources (endpoints).'),
  ];
  $items['restapi-rebuild'] = [
    'description' => dt('Rebuild the resource cache, reporting any errors found within resource definitions. This will also rebuild the menu.'),
  ];

  return $items;

}


/**
 * Returns a list of available endpoints.
 *
 */
function drush_restapi_list() {

  $endpoints   = 0;
  $resources   = restapi_get_resources();
  $last_module = NULL;
  $rows        = [
    [dt('Module'), dt('Path'), dt('Class')],
  ];
  $methods     = ['delete', 'get', 'patch', 'put', 'post'];

  foreach($resources as $resource) {
    if ($last_module != $resource->getModule()) {
      $module = $last_module = $resource->getModule();
    }
    else {
      $module = '';
    }
    $rows[] = [$module, $resource->getPath(), $resource->getClass()];

    foreach ($methods as $method) {
      $endpoints += method_exists($resource->getClass(), $method) ? 1 : 0;
    }
  }

  drush_print_table($rows, TRUE);
  drush_log(dt('Displaying !count total resources with !endpoints endpoints.', [
    '!count' => count($resources),
    '!endpoints' => $endpoints,
  ]), 'ok');

}


/**
 * Rebuilds the cache of endpoints, which will generate warnings or errors for
 * problematic resources.
 *
 */
function drush_restapi_rebuild() {

  $resources = restapi_get_resources(TRUE);
  menu_rebuild();

  drush_log(dt('!count resources have been rebuilt and cached.', [
    '!count' => count($resources),
  ]), 'ok');

}
