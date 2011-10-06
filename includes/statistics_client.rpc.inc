<?php

/**
 * Implements hook_xmlrpc().
 */
function statistics_client_xmlrpc() {
  $methods = array();
  $methods[] = array(
    'statisticsClient.info',
    'statistics_client_info',
    array('array'),
    t('Send information about this Drupal instance.')
  );
  $methods[] = array(
    'statisticsClient.query',
    'statistics_client_query',
    array('array', 'string'),
    t('Process query.')
  );
  return $methods;
}

/**
 * Provide technical information about this installation.
 */
function statistics_client_info() {
  $n_users = db_query("SELECT count(uid) FROM {users}")->fetchField();
  $software = $_SERVER['SERVER_SOFTWARE'];
  $phpversion = phpversion();
  $class = 'DatabaseTasks_' . Database::getConnection()->driver();
  $tasks = new $class();
  $dbname = '';
  $dbversion = '';
  $dbname = $tasks->name();
  $dbversion = Database::getConnection()->version();
  return array(
    'client_version' => '0.01',
    'time' => date('l jS \of F Y h:i:s A'),
    'drupal_version' => VERSION,
    'web_server' => $software,
    'php' => $phpversion,
    'db' => $dbname .  $dbversion,
    'n_users' => $n_users,
    'modules' => module_list(),
  );
}
/**
 * Process query and return result.
 *
 * Note: query must appear in whitelist.
 */
function statistics_client_query($query) {
  // TODO: regexp instead of normal comparison
  $result = db_query('SELECT id, query FROM {statistics_client_query_whitelist}');
  $flag = FALSE;
  foreach ($result as $record) {
    if ($query === $record->query) {
      $flag = TRUE;
    }
  }
  if (!$flag) {
    return array();
  }
  $tmp_res = db_query($query);
  $result = array();
  foreach ($tmp_res as $record) {
    $result[] = $record;
  }
  return $result;
}
