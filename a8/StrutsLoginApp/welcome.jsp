<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%-- Success view: personalized message with s:property --%>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Welcome</title>
    <style type="text/css">
        body { font-family: system-ui, Segoe UI, sans-serif; margin: 2rem; background: #e8f5e9; }
        .box { max-width: 520px; margin: 3rem auto; background: #fff; padding: 2rem; border-radius: 8px; border: 1px solid #c8e6c9; text-align: center; }
        h1 { color: #1b5e20; }
        p { color: #333; }
    </style>
</head>
<body>
<div class="box">
    <h1>Congratulations!</h1>
    <p>
        Welcome,
        <strong><s:property value="name"/></strong>.
    </p>
    <p>Your details were validated successfully.</p>
    <p><a href="login.jsp">Back to login</a></p>
</div>
</body>
</html>
