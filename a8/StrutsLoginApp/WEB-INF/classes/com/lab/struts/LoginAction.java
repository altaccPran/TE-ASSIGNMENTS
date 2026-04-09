package com.lab.struts;

import com.opensymphony.xwork2.ActionSupport;

import java.util.regex.Pattern;

/**
 * Struts 2 action for the login form.
 * Validation is implemented in {@link #validate()} (not XML) using {@link ActionSupport#addFieldError}.
 * Compatible with Struts 2.5.x + default interceptor stack (includes validation workflow).
 */
public class LoginAction extends ActionSupport {

    private static final long serialVersionUID = 1L;

    /** Display name (letters/spaces; not empty; not digits-only). */
    private String name;
    /** 10-digit mobile (India-style: starts with 6–9). Adjust regex if your course expects another format. */
    private String mobile;
    /** Email address. */
    private String email;

    @Override
    public void validate() {
        final String n = trimOrEmpty(name);
        final String m = trimOrEmpty(mobile);
        final String e = trimOrEmpty(email);

        // (d) Empty / missing values
        if (n.isEmpty()) {
            addFieldError("name", "Name is required.");
        } else {
            // (a) "Correct" name: letters, spaces, apostrophe, dot, hyphen; length >= 2
            if (!Pattern.matches("[a-zA-Z][a-zA-Z\\s'.-]{1,}", n)) {
                addFieldError("name", "Enter a valid name (start with a letter; letters/spaces only, min 2 chars).");
            } else if (Pattern.matches("\\d+", n.replaceAll("\\s+", ""))) {
                // Edge: purely numeric after removing spaces
                addFieldError("name", "Name cannot be only numbers.");
            }
        }

        if (m.isEmpty()) {
            addFieldError("mobile", "Mobile number is required.");
        } else if (!Pattern.matches("^[6-9]\\d{9}$", m)) {
            // (b) Mobile: exactly 10 digits, typical Indian mobile range
            addFieldError("mobile", "Enter a valid 10-digit mobile number (starts with 6–9).");
        }

        if (e.isEmpty()) {
            addFieldError("email", "Email ID is required.");
        } else if (!Pattern.matches(
                "^[A-Za-z0-9+_.-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$",
                e)) {
            // (c) Email format
            addFieldError("email", "Enter a valid email address (e.g. name@example.com).");
        }
    }

    /**
     * Called only when {@link #validate()} produced no field errors.
     */
    @Override
    public String execute() {
        return SUCCESS;
    }

    private static String trimOrEmpty(String s) {
        return s == null ? "" : s.trim();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getMobile() {
        return mobile;
    }

    public void setMobile(String mobile) {
        this.mobile = mobile;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }
}
