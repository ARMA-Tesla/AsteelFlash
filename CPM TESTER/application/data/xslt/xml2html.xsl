<?xml version="1.0" encoding="windows-1250" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://www.w3.org/2005/02/xpath-functions">
	<xsl:output method="html" />
	
	<xsl:template match="*"/>
	
	<xsl:template match="/testseq">
		<xsl:text disable-output-escaping="yes">&lt;!DOCTYPE html PUBLIC &quot;-//W3C//DTD XHTML 1.0 Strict//EN&quot; &quot;http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd&quot;&gt;</xsl:text>
		<html xmlns="http://www.w3.org/1999/xhtml">
			<head>
				<meta http-equiv="content-type" content="text/html; charset=windows-1250" />
				<link rel="stylesheet" type="text/css" href="css/screen.css" media="screen" />
				<link rel="stylesheet" type="text/css" href="css/print.css" media="print" />
				<script language="javascript">
					function show(id){
							document.getElementById("wind"+id).style.display = (document.getElementById("wind"+id).style.display == "") ? document.getElementById("wind"+id).style.display = "none" : document.getElementById("wind"+id).style.display = "";
						}
					function showFoto(url,id){
							document.getElementById("fotohl"+id).src = url;
							document.getElementById("fotobig"+id).src = url;
							document.getElementById("fotohlt"+id).innerHTML = url;
							document.getElementById("fotobigt"+id).innerHTML = url;
						}
					function showBigFoto(url,id){
							document.getElementById("fotohlbig"+id).style.display = (document.getElementById("fotohlbig"+id).style.display == "") ? document.getElementById("fotohlbig"+id).style.display = "none" : document.getElementById("fotohlbig"+id).style.display = "";
						}
				</script>
			</head>
			<body>
				<xsl:apply-templates select="product" />
				<table cellpadding="3" cellspacing="0" id="TTable">
					<colgroup>
						<col class="col-a" align="left" />
						<col class="col-b" align="left" />
						<col class="col-c" align="center" />
						<col class="col-d" align="center" />
						<col class="col-e" align="center" />
						<col class="col-f" align="center" />
						<col class="col-g" align="center" />
					</colgroup>
					<thead>
						<tr>
							<th>Test Name</th>
							<th>Test Description</th>
							<th>Value</th>
							<th>Low Limit</th>
							<th>High Limit</th>
							<th>Units</th>
							<th class="e">Duration</th>
						</tr>
					</thead>
					
					<tbody>
						<xsl:apply-templates select="process" />
						
					</tbody>
					
					<tfoot>
						<tr>
							<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
							<td>Moyenne Temps de test :</td>
							<td colspan="5"><xsl:value-of select="product/duration"/> s</td>
						</tr>
					</tfoot>
				</table>
				<br/><hr/>
				<a href="javascript:show('1_trace');">Software Traceability</a>
				<span id="wind1_trace" style="display: none;">
					<table cellpadding="3" cellspacing="0" id="TTable">
						<thead>
							<tr>
								<th>Type</th>
								<th>Description</th>
								<th>Version</th>
								<th>Built Time</th>
								<th class="e">GUID</th>
							</tr>
						</thead>
						<tbody>
							<xsl:apply-templates select="version" />
						</tbody>										
					</table>
				</span>
			</body>
		</html>
	</xsl:template>
	
	<xsl:template match="product">
		<table cellpadding="0" cellspacing="0" id="THead">
			<tbody>
				<tr>
					<td valign="middle" align="left" width="150"><img src="img/logo-asteel.jpg" width="80" height="39" /></td>
					<td valign="top" align="left">
						<p>
							<span class="label">Name:</span><xsl:text disable-output-escaping="yes">&#160;</xsl:text><span class="value"><xsl:value-of select="name"/></span><br />
							<span class="label">Barcode:</span><xsl:text disable-output-escaping="yes">&#160;</xsl:text><span class="value"><xsl:value-of select="barcode"/></span><br />
							<span class="label">Time:</span><xsl:text disable-output-escaping="yes">&#160;</xsl:text><span class="value"><xsl:value-of select="date_time"/></span>
						</p>
					</td>
					<td valign="middle" align="right" width="150"> <img src="img/logo-arma.gif" width="94" height="59"  /></td>
				</tr>
			</tbody>
		</table>
    </xsl:template>

    <xsl:template match="process">
	
		<xsl:call-template name="colorLine">
			<xsl:with-param name="color" select="'nok'"/>
		</xsl:call-template>
	 	
		<xsl:apply-templates select="test"/> <!-- ajoutée -->
		
		<xsl:for-each select="test">  <!-- ajoutée -->  
		<xsl:if test="used = 1">		
			<xsl:choose>
				<xsl:when test="failed = 0">
					<xsl:call-template name="colorLine">
						<xsl:with-param name="color" select="'ok'"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="colorLine">
						<xsl:with-param name="color" select="'error'"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
		 </xsl:for-each>  <!-- ajoutée -->
			
	</xsl:template>
	
	<xsl:template name="colorLine">
		<xsl:param name="color"/>
		<xsl:variable name="id_number">
			<xsl:number level="single"></xsl:number>
			<xsl:choose>
				<xsl:when test="name() = 'testseq_init'">
					<xsl:text>_init</xsl:text>
				</xsl:when>
				<xsl:when test="name() = 'testseq_cleanup'">
					<xsl:text>_clean</xsl:text>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:text disable-output-escaping="yes">&lt;tr&gt;</xsl:text>
			<!-- Colorize error test -->
			<xsl:choose>
				<xsl:when test="$color = 'error'">
					<xsl:text disable-output-escaping="yes">&lt;td class=&quot;</xsl:text>
					<xsl:value-of select="$color" />
					<xsl:text disable-output-escaping="yes">&quot; nowrap&gt;</xsl:text>
				</xsl:when>
				<xsl:when test="$color = 'nok'">
					<xsl:text disable-output-escaping="yes">&lt;td class=&quot;</xsl:text>
					<xsl:value-of select="$color" />
					<xsl:text disable-output-escaping="yes">&quot; nowrap&gt;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text disable-output-escaping="yes">&lt;td&gt;</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			
			<!-- add javascript when some additional data exists -->
			<xsl:if test="Log or Error or image">
				<xsl:text disable-output-escaping="yes">&lt;a href=&quot;javascript:show('</xsl:text>
				<xsl:value-of select="$id_number" />
				<xsl:text disable-output-escaping="yes">');</xsl:text>
				<xsl:if test="image">
					<xsl:text disable-output-escaping="yes">showFoto('</xsl:text>
					<xsl:value-of select="translate(image, '\', '/')" />
					<xsl:text disable-output-escaping="yes">',</xsl:text>
					<xsl:value-of select="format-number($id_number, '0')" />
					<xsl:text disable-output-escaping="yes">);</xsl:text>
				</xsl:if>
				<xsl:text disable-output-escaping="yes">&quot;&gt;</xsl:text>
			</xsl:if>
			
			<!-- Test name -->
			<xsl:choose>
				<xsl:when test="name() = 'testseq_init'">
					<xsl:text>Initalize</xsl:text>
				</xsl:when>
				<xsl:when test="name() = 'testseq_cleanup'">
					<xsl:text>Cleanup</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="name" />
				</xsl:otherwise>
			</xsl:choose>
			
			<!-- end of hyperlink -->
			<xsl:if test="Log or Error or image">
				<xsl:text disable-output-escaping="yes">&lt;/a&gt;</xsl:text>
			</xsl:if>
			
			<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			
			<!-- Description of test -->
			<xsl:choose>
				<xsl:when test="$color = 'error'">
					<xsl:text disable-output-escaping="yes">&lt;td class=&quot;</xsl:text>
					<xsl:value-of select="$color" />
					<xsl:text disable-output-escaping="yes">&quot; nowrap&gt;</xsl:text>
				</xsl:when>
				<xsl:when test="$color = 'nok'">
					<xsl:text disable-output-escaping="yes">&lt;td class=&quot;</xsl:text>
					<xsl:value-of select="$color" />
					<xsl:text disable-output-escaping="yes">&quot; nowrap&gt;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text disable-output-escaping="yes">&lt;td&gt;</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:choose>
				<xsl:when test="string-length(description) != 0">
					<xsl:value-of select="description" />
				</xsl:when>
				<xsl:otherwise>
					<xsl:text disable-output-escaping="yes">&#160;</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			
			<!-- specific test data -->
			<xsl:choose>
				<!-- Measured data -->
				<xsl:when test="measure">
					<xsl:call-template name="display-measure-test">
						<xsl:with-param name="color" select="$color" />
					</xsl:call-template>
				</xsl:when>
				<!-- Unknown tests or tests without any measured value -->
				<xsl:otherwise>
					<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
					<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
					<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
					<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
					<td class="e">
						<xsl:value-of select="duration"/>
					</td>
				</xsl:otherwise>
			</xsl:choose>
		<xsl:text disable-output-escaping="yes">&lt;/tr&gt;</xsl:text>
		<xsl:if test="Log or Error or image">
			<xsl:text disable-output-escaping="yes">&lt;tr id=&quot;wind</xsl:text>
			<xsl:value-of select="$id_number" />
			<xsl:text disable-output-escaping="yes">&quot; style=&quot;display: none;&quot;&gt;</xsl:text>
				<td colspan="7" class="tdInfo">
					<xsl:if test="image">
	                    <xsl:text disable-output-escaping="yes">&lt;div id=&quot;fotohlbig</xsl:text>
						<xsl:value-of select="$id_number" />
						<xsl:text disable-output-escaping="yes">&quot; name=&quot;fotohlbig</xsl:text>
						<xsl:value-of select="$id_number" />
						<xsl:text disable-output-escaping="yes">&quot; class="photoInfoBig" style="display: none;&quot;&gt;</xsl:text>
	                        <xsl:text disable-output-escaping="yes">&lt;div class=&quot;photoInfoBigInfo&quot;&gt;</xsl:text>
	                            <xsl:text disable-output-escaping="yes">&lt;div class=&quot;fotoClose&quot;&gt;&lt;a href=&quot;javascript:showBigFoto('fotohlt</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">','</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">')&quot;&gt;</xsl:text>
								<span>Close</span>
								<xsl:text disable-output-escaping="yes">&lt;/a&gt;&lt;/div&gt;</xsl:text>
	                            <xsl:text disable-output-escaping="yes">&lt;span id=&quot;fotobigt</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">&quot; name=&quot;fotobigt</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">&quot;&gt;&lt;/span&gt;</xsl:text>
	                        <xsl:text disable-output-escaping="yes">&lt;/div&gt;</xsl:text>
							<xsl:text disable-output-escaping="yes">&lt;div&gt;&lt;a href=&quot;javascript:showBigFoto('fotohlt</xsl:text>
							<xsl:value-of select="$id_number" />
							<xsl:text disable-output-escaping="yes">','</xsl:text>
							<xsl:value-of select="$id_number" />
							<xsl:text disable-output-escaping="yes">')&quot;&gt;&lt;img id=&quot;fotobig</xsl:text>
							<xsl:value-of select="$id_number" />
							<xsl:text disable-output-escaping="yes">&quot; name=&quot;fotobig</xsl:text>
							<xsl:value-of select="$id_number" />
							<xsl:text disable-output-escaping="yes">&quot; src=&quot;../xslt/img/dot.gif&quot; width=&quot;750&quot; height=&quot;563&quot; alt=&quot;Zoom Out&quot; /&gt;&lt;/a&gt;&lt;/div&gt;</xsl:text>
	                    	<xsl:text disable-output-escaping="yes">&lt;/div&gt;</xsl:text>
							<div class="photoInfo">
								<xsl:text disable-output-escaping="yes">&lt;span id=&quot;fotohlt</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">&quot; name=&quot;fotohlt</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">&quot;&gt;&lt;/span&gt;</xsl:text>
							</div>
							<div>
								<xsl:text disable-output-escaping="yes">&lt;a href=&quot;javascript:showBigFoto('fotohlt</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">','</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">')&quot;>&lt;img id=&quot;fotohl</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">&quot; name=&quot;fotohl</xsl:text>
								<xsl:value-of select="$id_number" />
								<xsl:text disable-output-escaping="yes">&quot; src=&quot;../xslt/img/dot.gif&quot; width=&quot;790&quot; height=&quot;300&quot; alt=&quot;Zoom In&quot; /&gt;&lt;/a&gt;</xsl:text>
							</div>
					</xsl:if>
					<xsl:if test="Log">
						<pre><xsl:value-of select="Log"/></pre>
					</xsl:if>
					<xsl:if test="Error">
						<pre>
							<xsl:text>Error Code: </xsl:text>
							<xsl:value-of select="Error/Code"/>
							<xsl:text disable-output-escaping="yes">&#13;&#10;</xsl:text>
							<xsl:text> Call Path: </xsl:text>
							<xsl:value-of select="Error/CallPath"/>
							<xsl:text disable-output-escaping="yes">&#13;&#10;</xsl:text>
							<xsl:text>   Message: </xsl:text>
							<xsl:value-of select="Error/Msg"/>
							<xsl:text disable-output-escaping="yes">&#13;&#10;</xsl:text>
						</pre>
					</xsl:if>
				</td>
            <xsl:text disable-output-escaping="yes">&lt;/tr&gt;</xsl:text>
		</xsl:if>
	</xsl:template>
	


	<xsl:template name="displayValue">
		<xsl:param name="param" />
		<xsl:param name="type" />	
		<xsl:choose>
			<xsl:when test="string-length($param) != 0">
				<xsl:choose>
					<!-- real64 -->
					<xsl:when test="$type = 'real64'">
						<xsl:choose>
							<xsl:when test="$param &lt; 0">
								<xsl:text>-</xsl:text>
								<xsl:call-template name="scient-format">
									<xsl:with-param name="number" select="substring($param, 2)" />
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="scient-format">
									<xsl:with-param name="number" select="$param" />
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<!-- int32 -->
					<xsl:when test="$type = 'int32'">
						<xsl:value-of select="format-number($param, '##0')"/>
					</xsl:when>
					<!-- string -->
					<xsl:when test="$type = 'string'">
						<xsl:value-of select="$param"/>
					</xsl:when>
					<!-- otherwise -->
					<xsl:otherwise>
						<xsl:value-of select="$type" />: <xsl:value-of select="$param"/>
					</xsl:otherwise>			
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text disable-output-escaping="yes">&#160;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>		
	</xsl:template>
	
	<xsl:template name="scient-format">
		<xsl:param name="number" />
		<xsl:param name="exponent" select="0" />	
		<xsl:choose>
			<xsl:when test="$number >= 1000.0">
				<xsl:call-template name="scient-format">
					<xsl:with-param name="number">
						<xsl:choose>
							<xsl:when test="contains($number, '.')">
								<xsl:value-of select="substring($number, 1, string-length(substring-before($number, '.')) - 3)" /> <!-- part greater when 1000 -->
								<xsl:text>.</xsl:text>
								<xsl:value-of select="substring(substring-before($number, '.'), string-length(substring-before($number, '.')) - 2)" /> <!-- part less when 1000 -->
								<xsl:value-of select="substring-after($number, '.')" />  <!-- part after . -->
							</xsl:when>
							<xsl:when test="contains($number, ',')">
								<xsl:value-of select="substring($number, 1, string-length(substring-before($number, ',')) - 3)" /> <!-- part greater when 1000 -->
								<xsl:text>,</xsl:text>
								<xsl:value-of select="substring(substring-before($number, ','), string-length(substring-before($number, ',')) - 2)" /> <!-- part less when 1000 -->
								<xsl:value-of select="substring-after($number, ',')" />  <!-- part after . -->
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="substring($number, 1, string-length($number) - 3)" /> <!-- part greater when 1000 -->
								<xsl:text>.</xsl:text>
								<xsl:value-of select="substring($number, string-length($number) - 3)" /> <!-- part less when 1000 -->
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="exponent" select="format-number($exponent + 3, '0')" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$number = 0">
				<xsl:value-of select="format-number($number, '#0.000')" />
			</xsl:when>
			<xsl:when test="$number &lt; 0.01">
				<xsl:call-template name="scient-format">
					<xsl:with-param name="number">
						<xsl:choose>
							<xsl:when test="contains($number, '.')">
								<xsl:value-of select="substring-before($number, '.')" /> <!-- part before . -->
								<xsl:value-of select="substring(substring-after($number, '.'), 1, 3)" /> <!-- part greater when 0.01 -->
								<xsl:text>.</xsl:text>
								<xsl:value-of select="substring(substring-after($number, '.'), 4)" />  <!-- part less 0.01 -->
							</xsl:when>
							<xsl:when test="contains($number, ',')">
								<xsl:value-of select="substring-before($number, '.')" /> <!-- part before . -->
								<xsl:value-of select="substring(substring-after($number, ','), 1, 3)" /> <!-- part greater when 0.01 -->
								<xsl:text>,</xsl:text>
								<xsl:value-of select="substring(substring-after($number, ','), 3)" />  <!-- part less 0.01 -->
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>0</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="exponent" select="format-number($exponent - 3, '0')" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="format-number($number, '#0.000')" />
				<xsl:if test="string-length($exponent) != 0">
					<xsl:choose>
						<xsl:when test="$exponent &gt; 0">
							<xsl:value-of select="format-number($exponent, 'e+00')" />
						</xsl:when>
						<xsl:when test="$exponent &lt; 0">
							<xsl:value-of select="format-number(-$exponent, 'e-00')" />
						</xsl:when>
					</xsl:choose>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="color-error-measurement">
		<xsl:param name="color" />
		<xsl:param name="value" />
		<xsl:param name="type" />
		<xsl:param name="low" />
		<xsl:param name="high" />		
		<xsl:choose>
			<xsl:when test="$color = 'error'">
				<xsl:choose>
					<xsl:when test="$type = 'real64' or 'int32'">
						<xsl:choose>
							<xsl:when test="$value &lt; $low or $value &gt; $high">
								<xsl:text disable-output-escaping="yes">&lt;td class=&quot;error&quot;&gt;</xsl:text>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text disable-output-escaping="yes">&lt;td&gt;</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text disable-output-escaping="yes">&lt;td class=&quot;error&quot;&gt;</xsl:text>						
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text disable-output-escaping="yes">&lt;td&gt;</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	
	<xsl:template name="display-measure-test">
		<xsl:param name="color" select="'ok'" />
  		<xsl:variable name="skipDesc">
			<xsl:choose>
				<xsl:when test="measure[@step = '1']/config and string-length(measure[@step = '1']/config) != 0">
				0
				</xsl:when>
				<xsl:otherwise>
				1
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="not(measure[used = 1])">
			<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
			<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
			<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
			<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
			<td  class="e">
				<xsl:value-of select="duration"/>
			</td>
			<xsl:text disable-output-escaping="yes">&lt;/tr&gt;</xsl:text>
		</xsl:if>
		<xsl:for-each select="measure[used = 1]">
			<xsl:if test="position() = 1 and config and string-length(config) != 0">
				<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
				<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
				<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
				<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
				<td  class="e">
					<xsl:value-of select="../duration"/>
				</td>
				<xsl:text disable-output-escaping="yes">&lt;/tr&gt;</xsl:text>
				<xsl:text disable-output-escaping="yes">&lt;tr&gt;</xsl:text>
				<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
			</xsl:if>
			<xsl:if test="position() > 1 or $skipDesc = 0">
				<xsl:call-template name="color-error-measurement">
					<xsl:with-param name="color" select="$color" />
					<xsl:with-param name="value" select="value" />
					<xsl:with-param name="type" select="value/@type" />
					<xsl:with-param name="low" select="limit/value1" />
					<xsl:with-param name="high" select="limit/value2" />
				</xsl:call-template>
				<xsl:value-of select="config" />
				<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			</xsl:if>				
			<xsl:call-template name="color-error-measurement">
				<xsl:with-param name="color" select="$color" />
				<xsl:with-param name="value" select="value" />
				<xsl:with-param name="type" select="value/@type" />
				<xsl:with-param name="low" select="limit/value1" />
				<xsl:with-param name="high" select="limit/value2" />
			</xsl:call-template>
			<xsl:call-template name="displayValue">
				<xsl:with-param name="param" select="value"/>
				<xsl:with-param name="type" select="value/@type"/>
			</xsl:call-template>
			<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			<xsl:call-template name="color-error-measurement">
				<xsl:with-param name="color" select="$color" />
				<xsl:with-param name="value" select="value" />
				<xsl:with-param name="type" select="value/@type" />
				<xsl:with-param name="low" select="limit/value1" />
				<xsl:with-param name="high" select="limit/value2" />
			</xsl:call-template>
			<xsl:call-template name="displayValue">
				<xsl:with-param name="param" select="limit/value1"/>
				<xsl:with-param name="type">string</xsl:with-param>
			</xsl:call-template>
			<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			<xsl:call-template name="color-error-measurement">
				<xsl:with-param name="color" select="$color" />
				<xsl:with-param name="value" select="value" />
				<xsl:with-param name="type" select="value/@type" />
				<xsl:with-param name="low" select="limit/value1" />
				<xsl:with-param name="high" select="limit/value2" />
			</xsl:call-template>
			<xsl:call-template name="displayValue">
				<xsl:with-param name="param" select="limit/value2"/>
				<xsl:with-param name="type">string</xsl:with-param>
			</xsl:call-template>
			<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			<xsl:call-template name="color-error-measurement">
				<xsl:with-param name="color" select="$color" />
				<xsl:with-param name="value" select="value" />
				<xsl:with-param name="type" select="value/@type" />
				<xsl:with-param name="low" select="limit/value1" />
				<xsl:with-param name="high" select="limit/value2" />
			</xsl:call-template>
			<xsl:call-template name="displayValue">
				<xsl:with-param name="param" select="unit"/>
				<xsl:with-param name="type">string</xsl:with-param>
			</xsl:call-template>
			<xsl:text disable-output-escaping="yes">&lt;/td&gt;</xsl:text>
			<td  class="e">
				<xsl:choose>
					<xsl:when test="position() = 1 and $skipDesc = 1">
						<xsl:value-of select="../duration"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:choose>
							<xsl:when test="duration and string-length(duration)">
								<xsl:value-of select="duration"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text disable-output-escaping="yes">&#160;</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</td>
			<xsl:if test="position() != last()">
				<xsl:text disable-output-escaping="yes">&lt;/tr&gt; &lt;tr&gt;</xsl:text>
				<td><xsl:text disable-output-escaping="yes">&#160;</xsl:text></td>
			</xsl:if>
		</xsl:for-each>			
	</xsl:template>

</xsl:stylesheet>
