<?php
/**
 * @file
 * Contains the api documentation and example of the module autosave_webform.
 *
 * @author Jaap Jansma (CiviCooP) <jaap.jansma@civicoop.org>
 *
 * @license http://www.gnu.org/licenses/agpl-3.0.html
 */

/**
 * Alter the list of forms configured for autosaving.
 *
 * To enable alter saving at a form add the form_id to the array of $form.
 *
 * @param array $forms
 *    The array with current defined forms.
 * @param array $form
 *    The form.
 * @param string $form_id
 *    The form id.
 * @param array $form_state
 *    The form state.
 *
 * @return array
 *   Array containing valid form ids for autosaving.
 */
function hook_autosavewebform_alter_autosaving_forms(array $forms, array $form, $form_id, array $form_state) {
  $forms['webform_client_form_1'] = array(
    // Set this to true when you don't want a clear saved
    // data on the submitting of your form.
    'no-submit-handler' => FALSE,
    // Set this to false if you don't want to autoload stored data on your form.
    'auto-restore' => TRUE,
  );
  return $forms;
}

/**
 * Alter the form_state and form after a restore has taken place.
 *
 * The $data contains the restored data and can be used to alter the $form_state.
 *
 * @param array $form
 *    The form.
 * @param string $form_id
 *    The form id.
 * @param array $form_state
 *    The form state.
 * @param array $data
 *    The saved data.
 */
function hook_autosavewebform_restore_alter(array &$form, $form_id, array &$form_state, array $data) {
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
 *    The form.
 * @param string $form_id
 *    The form id.
 * @param array $form_state
 *    The form state.
 * @param array $data
 *    The data to be saved.
 */
function hook_autosavewebform_save_alter(array &$form, $form_id, array &$form_state, array &$data) {
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
