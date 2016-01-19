<?php
/**
 * @file
 * This file contains the functionality to enable autosaving for webforms.
 *
 * @author Jaap Jansma (CiviCooP) <jaap.jansma@civicoop.org>
 *
 * @license http://www.gnu.org/licenses/agpl-3.0.html
 */

/**
 * Alter the form_state and form after a restore has taken place.
 *
 * The $data contains the restored data and can be used to
 * alter the $form_state.
 *
 * @param array $form
 *  The form.
 * @param string $form_id
 *  The form id.
 * @param array $form_state
 *  The form state.
 * @param array $data
 *  The saved data.
 */
function autosave_webform_autosavewebform_restore_alter(&$form, $form_id, &$form_state, $data) {
  if (isset($form_state['webform'])) {
    if (isset($data['values'])) {
      $form_state['values'] = $data['values'];
    }
    if (isset($data['webform'])) {
      $form_state['webform'] = $data['webform'];
    }
    if (isset($data['storage'])) {
      $form_state['storage'] = $data['storage'];
    }
  }
}

/**
 * Alter the data before saving it.
 *
 * You can use this hook to add additional data for storing.
 *
 * @param array $form
 *  The form.
 * @param string $form_id
 *  The form id.
 * @param array $form_state
 *  The form state.
 * @param array $data
 *  The saved data.
 */
function autosave_webform_autosavewebform_save_alter(&$form, $form_id, &$form_state, &$data) {
  if (isset($form_state['webform'])) {
    if (isset($form_state['values'])) {
      $data['values'] = $form_state['values'];
    }
    if (isset($form_state['webform'])) {
      $data['webform'] = $form_state['webform'];
    }
    if (isset($form_state['storage'])) {
      $data['storage'] = $form_state['storage'];
    }
  }
}

/**
 * Alter the list of forms configured for autosaving.
 *
 * To enable alter saving at a form add the form_id to the array of $form.
 *
 * @param array $forms
 *  Array of form_ids.
 * @param array $form
 *  The form.
 * @param string $form_id
 *  The form id.
 * @param array $form_state
 *  The form state.
 * @return array
 */
function autosave_webform_autosavewebform_alter_autosaving_forms($forms, $form, $form_id, $form_state) {
  $node = &$form['#node'];
  $enabled = isset($node->autosave_webform_enable) && $node->autosave_webform_enable ? true : false;
  if ($enabled) {
    $forms[] = $form_id;
  }
  return $forms;
}

/**
 * Adds a checkbox to webform settings page
 *
 * @param array $form
 *  The form.
 * @param string $form_id
 *  The form id.
 * @param array $form_state
 *  The form state.
 */
function autosave_webform_form_webform_configure_form_alter(&$form, $form_state, $form_id) {
  $node = &$form['#node'];
  $enabled = isset($node->autosave_webform_enable) && $node->autosave_webform_enable ? true : false;

  $form['autosave_webform'] = array(
    '#type' => 'fieldset',
    '#title' => t("Autosave webform"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['autosave_webform']['autosave_webform_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save this webform automaticly'),
    '#default_value' => $enabled,
  );

  $form['#submit'][] = 'autosave_webform_form_webform_configure_form_submit';
}

function _autosave_webform_webform_node_load($nodes, $types) {
  $db = db_query('SELECT * FROM {autosave_webforms} WHERE nid IN(:nids)', array(':nids' => array_keys($nodes)));
  foreach ($db as $autosave_webform) {
    $node = &$nodes[$autosave_webform->nid];
    $node->autosave_webform_enable = $autosave_webform->autosave_webform_enable ? true : false;
  }
}

function autosave_webform_form_webform_configure_form_submit(&$form, &$form_state)
{
  $node = &$form['#node'];
  $node->autosave_webform_enable = $form_state['input']['autosave_webform_enable'] ? true : false;
  node_save($node);
}

function _autosave_webform_webform_node_insert($node) {
  if (isset($node->autosave_webform_enable)) {
    db_insert('autosave_webforms')
      ->fields(array(
        'nid' => $node->nid,
        'autosave_webform_enable' => $node->autosave_webform_enable ? 1 : 0,
      ))
      ->execute();
  }
}

function _autosave_webform_webform_node_update($node) {
  if (isset($node->autosave_webform_enable)) {
    db_merge('autosave_webforms')
      ->key(array(
        'nid' => $node->nid,
      ))
      ->fields(array(
        'autosave_webform_enable' => $node->autosave_webform_enable ? 1 : 0,
      ))
      ->execute();
  }
}

/*
 * Implements hook_node_delete().
 *
 * @param object $node
 */
function _autosave_webform_webform_node_delete($node) {
  if (isset($node->autosave_webform_enable)) {
    db_delete('autosave_webforms')
      ->condition('nid', $node->nid)
      ->execute();
  }
}
