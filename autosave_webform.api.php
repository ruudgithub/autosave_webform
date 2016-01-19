<?php
/**
 * @author Jaap Jansma (CiviCooP) <jaap.jansma@civicoop.org>
 * @license http://www.gnu.org/licenses/agpl-3.0.html
 */

/**
 * Alter the list of forms configured for autosaving.
 *
 * To enable alter saving at a form add the form_id to the array of $form;
 *
 * @param $forms array
 * @param $form
 * @param $form_id
 * @param $form_state
 * @return array
 */
function hook_autosavewebform_alter_autosaving_forms($forms, $form, $form_id, $form_state) {
    $forms['webform_client_form_1'] = array(
        'no-submit-handler' => false, //set this to true when you don't want a clear saved data on the submitting of your form
        'auto-restore' => true,   //set this to false if you don't want to autoload stored data on your form
    );
    return $forms;
}

/**
 * Alter the form_state and form after a restore has taken place.
 *
 * The $data contains the restored data and can be used to alter the $form_state
 *
 * @param $form
 * @param $form_id
 * @param $form_state
 * @param $data
 */
function hook_autosavewebform_restore_alter(&$form, $form_id, &$form_state, $data) {
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
 * Alter the data before saving it
 *
 * You can use this hook to add additional data for storing.
 *
 * @param $form
 * @param $form_id
 * @param $form_state
 * @param $data
 */
function hook_autosavewebform_save_alter(&$form, $form_id, &$form_state, &$data) {
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