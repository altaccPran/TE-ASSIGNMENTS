<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%--
  Login form using Struts UI tags.
  Field errors (validate()) appear via s:fielderror / s:actionerror.
--%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Login — Struts 2 Lab</title>
    <s:head/>
    <style type="text/css">
        body { font-family: system-ui, Segoe UI, sans-serif; margin: 2rem; background: #f4f6f8; }
        .box { max-width: 420px; margin: 0 auto; background: #fff; padding: 1.5rem; border-radius: 8px; border: 1px solid #ddd; }
        h1 { font-size: 1.25rem; margin-top: 0; }
        .hint { font-size: 0.85rem; color: #555; margin-bottom: 1rem; }
    </style>
</head>
<body>
<div class="box">
    <h1>Login</h1>
    <p class="hint">Name, mobile (10 digits, 6–9 first), and email. Struts re-displays values and errors on failure.</p>

    <s:actionerror/>

    <s:form action="login" method="post" theme="xhtml">
        <s:textfield name="name" label="Name" placeholder="Your full name"/>
        <s:fielderror fieldName="name"/>

        <s:textfield name="mobile" label="Mobile number" placeholder="10-digit mobile"/>
        <s:fielderror fieldName="mobile"/>

        <s:textfield name="email" label="Email ID" placeholder="you@example.com"/>
        <s:fielderror fieldName="email"/>

        <s:submit value="Login"/>
    </s:form>
</div>
</body>
</html>
