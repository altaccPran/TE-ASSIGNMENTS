# Assignment 9 — AngularJS login & registration SPA (macOS)

Single-page application built with **AngularJS 1.x** (no Node.js, npm, or Angular CLI). Uses a CDN for the framework and **`ng-show` / `ng-hide`-style toggling** so the app runs when opened from disk.

## Files

| File | Purpose |
|------|---------|
| `index.html` | Shell, `ng-app`, `ng-controller`, login & registration forms, validation |
| `app.js` | Module `authApp`, `AuthController`, view switching, dummy `loginUser` / `registerUser` |
| `style.css` | Layout, validation styles, panel transitions |

## How to test (macOS only)

1. **Finder**  
   Open the folder containing this assignment (e.g. `TE-ASSIGNMENTS/a9`).

2. **Open in browser**  
   **Double-click `index.html`** (or right-click → **Open With** → **Safari**, **Google Chrome**, or **Firefox**).  
   The page loads Angular from the CDN and `app.js` / `style.css` from the same folder next to `index.html`.

3. **What to try**  
   - Switch **Login** / **Register** with the tabs — the page does not reload.  
   - Leave fields empty or too short: inline errors appear; **Submit** stays disabled until the form is valid and touched (`$dirty` / `$invalid` as wired).  
   - Submit valid data: a browser **`alert()`** confirms login or registration, then fields clear.

## Why not `$routeProvider` / `ngRoute` here?

`angular-route` often loads partial templates with **`$http`**. With the **`file://`** protocol, browsers treat many requests as cross-origin, which can block those requests (CORS / file access). This lab keeps everything in one HTML file and switches views with **`ng-show`** / **`ng-class`**, so **double-clicking `index.html` works reliably** on macOS without a local web server.

*(Optional: if you later serve the folder with a tiny HTTP server, routing can be added; it is not required for this lab.)*

## Requirements checklist (lab)

- [x] Registration: first name, last name, username, password  
- [x] Login: username, password  
- [x] SPA: toggle views without full page reload  
- [x] Form validation: `ng-required`, `$valid` / `$invalid`, `$pristine` / `$dirty`, disabled submit until valid  
- [x] `registerUser()` / `loginUser()` dummy success → `alert()` + clear models  

## References (AngularJS 1.x)

- [AngularJS Developer Guide (archive)](https://docs.angularjs.org/guide)  
- [AngularJS API: ngModel, forms](https://docs.angularjs.org/api/ng/directive/ngModel)  
- [W3Schools AngularJS](https://www.w3schools.com/angular/angular_intro.asp)  

Note: **`angular.io`** documents **Angular (v2+)**; this assignment uses **AngularJS (1.x)** — different framework names and APIs.
