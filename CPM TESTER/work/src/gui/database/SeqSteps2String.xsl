<!-- Sequence steps list XML to BKTree string (single step) conversion style sheet -->
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
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="step">
		<xsl:value-of select="@fn"/>
		<xsl:text>;</xsl:text>
		<xsl:for-each select="@*">
			<xsl:if test="name()!='fn'">
				<xsl:value-of select="name()"/>
				<xsl:if test="not(position()=last())">;</xsl:if>
			</xsl:if>
		</xsl:for-each>
		<xsl:text>~#10~</xsl:text>
	</xsl:template>

</xsl:stylesheet>
