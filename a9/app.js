/**
 * AngularJS 1.x — Auth SPA (login + registration)
 * Module + single controller; view switching via $scope.activeView (no $routeProvider).
 */

(function () {
  'use strict';

  // Root module for this tiny app
  angular.module('authApp', []);

  angular.module('authApp').controller('AuthController', AuthController);

  AuthController.$inject = ['$scope'];

  function AuthController($scope) {
    // -------------------------------------------------------------------------
    // View state: 'login' | 'register' — drives ng-show on each panel
    // -------------------------------------------------------------------------
    $scope.activeView = 'login';

    /**
     * Switch visible panel without reloading the page.
     * @param {string} view - 'login' or 'register'
     */
    $scope.setView = function (view) {
      $scope.activeView = view;
    };

    // -------------------------------------------------------------------------
    // Model objects bound to forms via ng-model
    // -------------------------------------------------------------------------
    $scope.login = {
      username: '',
      password: ''
    };

    $scope.register = {
      firstName: '',
      lastName: '',
      username: '',
      password: ''
    };

    /**
     * Dummy login: validates via Angular form object, then alert + reset.
     * @param {Object} form - Angular form controller (e.g. loginForm)
     */
    $scope.loginUser = function (form) {
      if (!form || form.$invalid) {
        return;
      }
      var u = $scope.login.username;
      alert('Welcome back, ' + u + '! You are now logged in (demo).');
      // Clear fields and reset validation state
      $scope.login.username = '';
      $scope.login.password = '';
      form.$setPristine();
      form.$setUntouched();
    };

    /**
     * Dummy registration: alert success, clear fields.
     * @param {Object} form - Angular form controller (registerForm)
     */
    $scope.registerUser = function (form) {
      if (!form || form.$invalid) {
        return;
      }
      var r = $scope.register;
      alert(
        'Registration successful (demo)!\n\n' +
          'Name: ' +
          r.firstName +
          ' ' +
          r.lastName +
          '\nUsername: ' +
          r.username
      );
      $scope.register.firstName = '';
      $scope.register.lastName = '';
      $scope.register.username = '';
      $scope.register.password = '';
      form.$setPristine();
      form.$setUntouched();
    };
  }
})();
