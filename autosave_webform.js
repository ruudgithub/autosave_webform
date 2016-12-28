/**
 * @file
 * This javascript provides the behaviour for autosaving webforms.
 *
 * The file consist of two parts the first part attach the behaviour to drupal forms.
 * The second part is the class and functionality for saving.
 */

(function () {

  "use strict";

  (function ($) {
    Drupal.behaviors.autosaveWebform = {
      attach: function (context, settings) {
        if (Drupal.settings.autosave_webform.form_id) {

          /**
           * Override default error handling so that when a request is in progress and the user
           * leaves the form he or she won't see a popup about a failing ajax request.
           *
           * @param response
           * @param uri
           */
          Drupal.ajax[Drupal.settings.autosave_webform.autosave_button_id].error = function (response, uri) {
            // Remove the progress element.
            if (this.progress.element) {
              $(this.progress.element).remove();
            }
            if (this.progress.object) {
              this.progress.object.stopMonitoring();
            }
            // Undo hide.
            $(this.wrapper).show();
            // Re-enable the element.
            $(this.element).removeClass('progress-disabled').removeAttr('disabled');
            // Reattach behaviors, if they were detached in beforeSerialize().
            if (this.form) {
              var settings = response.settings || this.settings || Drupal.settings;
              Drupal.attachBehaviors(this.form, settings);
            }
          };

          jQuery('#' + Drupal.settings.autosave_webform.form_id).autosave_webform({
            timeout: Drupal.settings.autosave_webform.timeout,
            autosave_button: Drupal.settings.autosave_webform.autosave_button
          });
        }
      }
    };

    var $autosave_webform = (function () {
      var params = {
        instantiated: null,
        started: null
      };

      function init() {

        return {
          setInitialOptions: function (options) {
            var defaults = {
              timeout: 15,
              name: null,
              autosave_button: 'edit-autosave',
              onSave: function () {
              }
            };
            this.options = this.options || $.extend(defaults, options);
          },

          setOptions: function (options) {
            this.options = this.options || this.setInitialOptions(options);
            this.options = $.extend(this.options, options);
          },

          start: function (targets) {
            targets = targets || {};
            var self = this;
            this.targets = this.targets || [];
            this.targets = $.merge(this.targets, targets);
            this.targets = $.unique(this.targets);
            this.targets = $(this.targets);
            if (!params.started) {
              params.started = true;
              self.saveDataByTimeout();
            }
          },

          saveAllData: function () {
            var self = this;
            $(self.options.autosave_button).mousedown();

            if ($.isFunction(self.options.onSave)) {
              self.options.onSave.call();
            }
            self.saveDataByTimeout();
          },

          saveDataByTimeout: function () {
            var self = this;
            setTimeout((function () {
              function timeout() {
                self.saveAllData();
              }

              return timeout;
            })(), self.options.timeout * 1000);
          }
        };
      }

      return {
        getInstance: function () {
          if (!params.instantiated) {
            params.instantiated = init();
            params.instantiated.setInitialOptions();
          }
          return params.instantiated;
        },

        free: function () {
          params = {};
          return null;
        }
      };
    })();

    $.autosave_webform = function () {
      return $autosave_webform.getInstance();
    };

    $.fn.autosave_webform = function (options) {
      var autosave_webform = $autosave_webform.getInstance();
      autosave_webform.setOptions(options);
      autosave_webform.start(this);
      return autosave_webform;
    };

    $.fn.autosave_webform_show_message = function () {
      var el = $(this);
      el.css("display", "");
      setTimeout(function () {
        el.css("display", "none");
      }, 3000);
    };

  })(jQuery);

})();
