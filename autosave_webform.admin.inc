<?php
/**
 * @file
 * Configuration page for autosave_webform
 *
 * @author ruudgithub
 *
 * @license http://www.gnu.org/licenses/agpl-3.0.html
 */
function autosave_webform_admin_settings($form, &$form_state) {
  $form['autosave_webform_admin_settings_tabs'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Autosave Webform Settings'),
  );

  $form['autosave_webform_admin_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autosave Webform Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => '',
    '#group' => 'autosave_webform_admin_settings_tabs',
  );

  $form['autosave_webform_admin_settings']['autosave_webform_autoremovedataactive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically remove autosave data after certain period'),
    '#default_value' => variable_get('autosave_webform_autoremovedataactive', NULL),
    '#description' => t('If this option is checked, autosave data older then the specified period below will automatically be removed from the database. This will prevent that the forms store endless amounts of data.'),
  );

  $form['autosave_webform_admin_settings']['autosave_webform_autoremoveperiod'] = array(
    '#type' => 'select',
    '#title' => t('Autoremoval period'),
    '#options' => array(
      '1' => t('1 month'),
      '3' => t('3 months'),
      '6' => t('6 months'),
      '12' => t('12 months'),
    ),
    '#default_value' => variable_get('autosave_webform_autoremoveperiod', 0),
    '#description' => t('Select the period in which the autosave data should remain. After this period, the autosave data is automatically removed (only if the checkbox above is checked).'),
  );

  return system_settings_form($form);
}