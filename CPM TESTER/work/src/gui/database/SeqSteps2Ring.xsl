<!-- Sequence steps list XML to BKTree string (selection ring) conversion style sheet -->
<!-- DO NOT MODIFY IT -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="text"/>

  <xsl:template match="*" />
	<xsl:strip-space elements="*"/>

  <xsl:template match="/configuration">
		<xsl:apply-templates />
	</xsl:template>
	
	<xsl:template match="plugins">
		<xsl:apply-templates />
	</xsl:template>
	
	<xsl:template match="plugin">
		<xsl:apply-templates />
	</xsl:template>
	
	<xsl:template match="steps">
		<xsl:text>~#01~not selected~#09~</xsl:text>
		<xsl:apply-templates />
		<xsl:text>@0</xsl:text>
	</xsl:template>

	<xsl:template match="step">
		<xsl:value-of select="@fn"/>
		<xsl:if test="not(position()=last())">~#09~</xsl:if>
	</xsl:template>

</xsl:stylesheet>
