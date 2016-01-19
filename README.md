# Drupal module autosave_webform
Drupal module for automaticlly save drafts of a webform in Drupal.

This module saves an open instance of a webform automatically in the database. 
When the user does not complete the form but does come back at a later moment 
the webform is prefilled with the data entered previously.

This module does the same thing as the module 
[Form Save State](https://www.drupal.org/project/form_save_state) but 
instead of saving the data into the browsers data store it saves the data in 
the drupal database. This way we can assure that the user still has the data 
available when he or she logs in from another computer.

## Technicall documentation

If you want to implement the auto save functionality in your own module you 
have the implement the following hooks:

* hook_autosavewebform_alter_autosaving_forms
* hook_autosavewebform_restore_alter
* hook_autosavewebform_save_alter

See [autosave_webform.api.php](autosave_webform.api.php) for more information.
