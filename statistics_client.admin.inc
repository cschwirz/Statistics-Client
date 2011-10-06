<?php

/**
 * @file
 * Administration page callbacks for the Statistics Client module.
 */

function statistics_client_form_query_table() {
  $header = array('Query', 'Description');
  $rows = array();
  $result = db_query('SELECT id, query, description FROM {statistics_client_query_whitelist}');
  foreach ($result as $record) {
    $row = array($record->query, $record->description);
    $rows['id' . $record->id] = $row;
  }
  $form['query_table'] = array
  (
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No queries in list.'),
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => 'Delete',
  );
  return $form;
}
function statistics_client_form_query_table_submit($form, &$form_state) {
  $result = db_query('SELECT id FROM {statistics_client_query_whitelist}');
  foreach ($result as $record) {
    if ($form_state['values']['query_table']['id' . $record->id]) {
      db_delete('statistics_client_query_whitelist')->condition('id', $record->id)->execute();
    }
  }
}
function statistics_client_form_add_query() {
  $form['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query'),
    '#description' => t('allowed query that may be called via RPC'),
    '#size' => 40,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#size' => 40,
    '#maxlength' => 256,
    '#required' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => 'Add query',
  );
  return $form;
}
function statistics_client_form_add_query_submit($form, &$form_state) {
  $query = $form_state['values']['query'];
  $description = $form_state['values']['description'];
  $id = db_insert('statistics_client_query_whitelist')
    ->fields(array(
          'query' => $query,
          'description' => $description,
          ))
    ->execute();
}

/**
 * Give form to setup whitelist of allowed queries.
 */
function statistics_client_admin_settings() {
  $page['form1'] = drupal_get_form('statistics_client_form_query_table');
  $page['form2'] = drupal_get_form('statistics_client_form_add_query');
  return $page;
}
