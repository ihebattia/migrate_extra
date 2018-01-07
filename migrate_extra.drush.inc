<?php
/**
 * Implements hook_drush_command().
 */

use Symfony\Component\Finder\Finder;
use Yosymfony\ResourceWatcher\ResourceWatcher;
use Yosymfony\ResourceWatcher\ResourceCacheFile;

function migrate_extra_drush_command() {

  $commands['migrate-debug-watch'] = [
    'description' => 'Watch the migration config files and import new changes.',
    'aliases' => ['mdw'],
    'arguments' => [
       'module_name' => 'My migration module name.',
       'target' => 'A Comma-separated list of migration groups to import.',
     ],
     'options' => [
       'limit' => 'Specify the number of records to import.',
       'dependencies' => 'Specify the depdencies to run first.',
     ],
     'examples' => [
       'drush mdw module_name migrate_drupal_6_node_user' => 'Import new users given the migrate_drupal_6 group.',
       'drush mdw module_name migrate_drupal_6_node_page --limit=10' => 'Import only 10 pages',
       'drush mdw module_name migrate_drupal_6_node_law --limit=10 --dependencies="upgrade_d6_user"' => 'Import 10 laws given specified depdencies',
     ],
  ];

  $commands['migrate-debug-cycle'] = [
    'description' => 'Full cycle of migration.',
    'aliases' => ['mdc'],
    'arguments' => [
       'module_name' => 'My migration module name.',
       'target' => 'A Comma-separated list of migration groups to import.',
     ],
     'options' => [
       'limit' => 'Specify the number of records to import.',
       'dependencies' => 'Specify the depdencies to run first',
     ],
     'examples' => [
       'drush mdc' => 'Print my example command.',
       'drush mdc module_name migrate_drupal_6_node_loi' => 'Import new nodes law given the migrate_drupal_6 group.',
       'drush mdc module_name migrate_drupal_6_node_loi --dependencies="upgrade_d6_user" --limit=10' => 'Import no more than 10 laws given the user depdency',
     ],
  ];

  $commands['migrate-debug-reload'] = [
    'description' => 'Reload cycle of migration.',
    'aliases' => ['mdr'],
    'arguments' => [
       'module_name' => 'My migration module name.',
     ],
     'options' => [
       'dependencies' => 'Specify the depdencies to run first',
     ],
     'examples' => [
       'drush mdr module_name' => 'Reload page nodes.',
     ],
  ];

  return $commands;
}

/**
 * Drush command logic.
 * drush_[MODULE_NAME]_[COMMAND_NAME]().
 */

function drush_migrate_extra_migrate_debug_watch($module_name, $target) {
  $finder = new Finder();
  $finder->files()
      ->name('*.yml')
      ->in(__DIR__ . '/cache');
  $rc = new ResourceCacheFile(__DIR__ . 'cache/cache-file.php');
  $rw = new ResourceWatcher($rc);
  $rw->setFinder($finder);

  while (true) {
    $rw->findChanges();
    if ($rw->hasChanges()) {
       drush_print("The migration config files has been changed.");
       drush_migrate_extra_migrate_debug_cycle($module_name, $target);
     }
    sleep(4);
  }
}

function drush_migrate_extra_migrate_debug_cycle($module_name, $target = null) {
  $limit = drush_get_option('limit', -1);
  drush_migrate_extra_migrate_debug_reload($module_name,$target);
  $opts = array();
  if($limit != -1) {
    $opts['limit'] = $limit;
  }
  drush_invoke_process('@self', 'mi', array($target), $opts);
  drush_print("The migration has been cycled.");
}

function drush_migrate_extra_migrate_debug_reload($module_name, $target) {
  $dependencies = drush_get_option('dependencies', array());
  if (!is_array($dependencies)) {
    $dependencies = explode(',', $dependencies);
  }
  refresh_migration_config($module_name, $dependencies, $target);
}

function refresh_migration_config($module_name, $dependencies, $target) {
  if (isset($target)) {
    drush_invoke_process('@self', 'migrate-rollback', array($target));
  }
  else {
    drush_invoke_process('@self', 'migrate-rollback', array(), array('--all'));
  }
  drush_invoke_process('@self', 'cdi', array($module_name));
  foreach ($dependencies as $key => $dependency) {
    drush_invoke_process('@self', 'mi', array($dependency));
  }
  drush_print("The migration has been refreshed.");
}
