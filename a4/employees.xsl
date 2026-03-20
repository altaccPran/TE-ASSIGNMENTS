<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">

<html>
<body>

<h2>Employee Table (Sorted by Experience - Descending)</h2>

<table border="1">

<tr>
<th>ID</th>
<th>Name</th>
<th>Department</th>
<th>Designation</th>
<th>Salary</th>
<th>Experience</th>
<th>Email</th>
<th>Joining Year</th>
</tr>

<xsl:for-each select="employees/employee">

    <!-- SORT BY EXPERIENCE -->
    <xsl:sort select="experience" order="descending" data-type="number"/>

<tr>

<td><xsl:value-of select="@empId"/></td>
<td><xsl:value-of select="name"/></td>
<td><xsl:value-of select="department"/></td>
<td><xsl:value-of select="designation"/></td>

<!-- Salary with condition -->
<td>
    <xsl:attribute name="style">
        <xsl:choose>
            <xsl:when test="salary &gt; 60000">
                background-color:lightgreen;
            </xsl:when>
            <xsl:otherwise>
                background-color:lightcoral;
            </xsl:otherwise>
        </xsl:choose>
    </xsl:attribute>
    <xsl:value-of select="salary"/>
</td>

<td><xsl:value-of select="experience"/></td>
<td><xsl:value-of select="email"/></td>

<!-- Show only Year -->
<td>
    <xsl:value-of select="substring(joiningDate,1,4)"/>
</td>

</tr>

</xsl:for-each>

</table>

</body>
</html>

</xsl:template>

</xsl:stylesheet>