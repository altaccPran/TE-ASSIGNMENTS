<?xml version="1.0" encoding="UTF-8"?>

<!--
  Transforms employees.xml into a single HTML page with a responsive data table.
  Open employees.xml in Firefox (recommended) with only the XSL xml-stylesheet PI active,
  or serve the folder over HTTP if your browser blocks file:// XSLT.
-->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:output method="html" encoding="UTF-8" indent="yes"/>

    <xsl:template match="/employees">
        <html lang="en">
            <head>
                <meta charset="utf-8"/>
                <meta name="viewport" content="width=device-width, initial-scale=1"/>
                <title>Employee Directory</title>
                <style type="text/css">
                    * { box-sizing: border-box; }
                    body {
                        margin: 0;
                        font-family: system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
                        background: #f1f5f9;
                        color: #0f172a;
                    }
                    .wrap {
                        max-width: 72rem;
                        margin: 0 auto;
                        padding: 1.5rem 1rem 2.5rem;
                    }
                    h1 {
                        font-size: 1.5rem;
                        font-weight: 700;
                        margin: 0 0 1rem;
                        letter-spacing: -0.02em;
                    }
                    .sub {
                        margin: 0 0 1.25rem;
                        font-size: 0.95rem;
                        color: #475569;
                    }
                    .table-scroll {
                        overflow-x: auto;
                        border-radius: 10px;
                        border: 1px solid #cbd5e1;
                        background: #fff;
                        box-shadow: 0 4px 14px rgba(15, 23, 42, 0.08);
                    }
                    table {
                        width: 100%;
                        border-collapse: collapse;
                        min-width: 52rem;
                        font-size: 0.9rem;
                    }
                    thead th {
                        text-align: left;
                        background: #1e293b;
                        color: #f8fafc;
                        font-weight: 600;
                        padding: 0.65rem 0.85rem;
                        white-space: nowrap;
                    }
                    tbody td {
                        padding: 0.6rem 0.85rem;
                        border-bottom: 1px solid #e2e8f0;
                        vertical-align: top;
                    }
                    tbody tr:nth-child(even) { background: #f8fafc; }
                    tbody tr:hover { background: #e0f2fe; }
                    tbody tr:last-child td { border-bottom: none; }
                    td.num { text-align: right; font-variant-numeric: tabular-nums; }
                </style>
            </head>
            <body>
                <div class="wrap">
                    <h1>Employee Directory</h1>
                    <p class="sub">Organization roster (tabular view via XSLT).</p>
                    <div class="table-scroll">
                        <table>
                            <thead>
                                <tr>
                                    <th>Employee ID</th>
                                    <th>First name</th>
                                    <th>Last name</th>
                                    <th>Department</th>
                                    <th>Designation</th>
                                    <th>Salary</th>
                                    <th>Email</th>
                                </tr>
                            </thead>
                            <tbody>
                                <xsl:for-each select="employee">
                                    <tr>
                                        <td><xsl:value-of select="@employeeId"/></td>
                                        <td><xsl:value-of select="firstName"/></td>
                                        <td><xsl:value-of select="lastName"/></td>
                                        <td><xsl:value-of select="department"/></td>
                                        <td><xsl:value-of select="designation"/></td>
                                        <td class="num"><xsl:value-of select="salary"/></td>
                                        <td><xsl:value-of select="email"/></td>
                                    </tr>
                                </xsl:for-each>
                            </tbody>
                        </table>
                    </div>
                </div>
            </body>
        </html>
    </xsl:template>

</xsl:stylesheet>
