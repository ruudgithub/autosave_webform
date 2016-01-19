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
 *   The form.
 * @param string $form_id
 *   The form id.
 * @param array $form_state
 *   The form state.
 * @param array $data
 *   The saved data.
 */
function autosave_webform_autosavewebform_restore_alter(array &$form, $form_id, array &$form_state, array $data) {
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
 *   The form.
 * @param string $form_id
 *   The form id.
 * @param array $form_state
 *   The form state.
 * @param array $data
 *   The saved data.
 */
function autosave_webform_autosavewebform_save_alter(array &$form, $form_id, array &$form_state, array &$data) {
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
 *   Array of form_ids.
 * @param array $form
 *   The form.
 * @param string $form_id
 *   The form id.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   An array with valid form_ids.
 */
function autosave_webform_autosavewebform_alter_autosaving_forms(array $forms, array $form, $form_id, array $form_state) {
  $node = &$form['#node'];
  $enabled = isset($node->autosave_webform_enable) && $node->autosave_webform_enable ? TRUE : FALSE;
  if ($enabled) {
    $forms[] = $form_id;
  }
  return $forms;
}

/**
 * Adds a checkbox to webform settings page.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $form_id
 *   The form id.
 */
function autosave_webform_form_webform_configure_form_alter(array &$form, array $form_state, $form_id) {
  $node = &$form['#node'];
  $enabled = isset($node->autosave_webform_enable) && $node->autosave_webform_enable ? TRUE : FALSE;

  $form['autosave_webform'] = array(
    '#type' => 'fieldset',
    '#title' => t("Autosave webform"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['autosave_webform']['autosave_webform_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save this webform automaticly'),
    '#default_value' => $enabled,
  );

  $form['#submit'][] = 'autosave_webform_form_webform_configure_form_submit';
}

/**
 * Deferred hook_node_load for webforms.
 *
 * @param array $nodes
 *    Array of nodes.
 * @param array $types
 *    Array of types.
 */
function _autosave_webform_webform_node_load(array &$nodes, array $types) {
  $db = db_query('SELECT * FROM {autosave_webforms} WHERE nid IN(:nids)', array(':nids' => array_keys($nodes)));
  foreach ($db as $autosave_webform) {
    $node = &$nodes[$autosave_webform->nid];
    $node->autosave_webform_enable = $autosave_webform->autosave_webform_enable ? TRUE : FALSE;
  }
}

/**
 * Deferred hook_webform_configure_form_submit.
 *
 * @param array $form
 *    The form.
 * @param array $form_state
 *    The form state.
 *
 * @throws Exception
 *    Throws an exception when node save fails.
 */
function autosave_webform_form_webform_configure_form_submit(array &$form, array &$form_state) {
  $node = &$form['#node'];
  $node->autosave_webform_enable = $form_state['input']['autosave_webform_enable'] ? TRUE : FALSE;
  node_save($node);
}

/**
 * Implements hook_node_insert().
 */
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

/**
 * Implements hook_node_update().
 */
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

/**
 * Implements hook_node_delete().
 */
function _autosave_webform_webform_node_delete($node) {
  if (isset($node->autosave_webform_enable)) {
    db_delete('autosave_webforms')
      ->condition('nid', $node->nid)
      ->execute();
  }
}
