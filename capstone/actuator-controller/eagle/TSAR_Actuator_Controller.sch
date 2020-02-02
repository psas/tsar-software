<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.5.1">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="mm" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="TSAR_Controller">
<packages>
<package name="LQFP64">
<smd name="1" x="-3.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="2" x="-3.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="3" x="-2.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="4" x="-2.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="5" x="-1.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="6" x="-1.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="7" x="-0.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="8" x="-0.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="9" x="0.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="10" x="0.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="11" x="1.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="12" x="1.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="13" x="2.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="14" x="2.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="15" x="3.25" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="16" x="3.75" y="-5.75" dx="1.2" dy="0.3" layer="1" rot="R90"/>
<smd name="17" x="5.75" y="-3.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="18" x="5.75" y="-3.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="19" x="5.75" y="-2.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="20" x="5.75" y="-2.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="21" x="5.75" y="-1.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="22" x="5.75" y="-1.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="23" x="5.75" y="-0.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="24" x="5.75" y="-0.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="25" x="5.75" y="0.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="26" x="5.75" y="0.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="27" x="5.75" y="1.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="28" x="5.75" y="1.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="29" x="5.75" y="2.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="30" x="5.75" y="2.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="31" x="5.75" y="3.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="32" x="5.75" y="3.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="33" x="3.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="34" x="3.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="35" x="2.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="36" x="2.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="37" x="1.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="38" x="1.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="39" x="0.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="40" x="0.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="41" x="-0.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="42" x="-0.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="43" x="-1.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="44" x="-1.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="45" x="-2.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="46" x="-2.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="47" x="-3.25" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="48" x="-3.75" y="5.75" dx="1.2" dy="0.3" layer="1" rot="R270"/>
<smd name="64" x="-5.75" y="-3.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="63" x="-5.75" y="-3.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="62" x="-5.75" y="-2.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="61" x="-5.75" y="-2.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="60" x="-5.75" y="-1.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="59" x="-5.75" y="-1.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="58" x="-5.75" y="-0.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="57" x="-5.75" y="-0.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="56" x="-5.75" y="0.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="55" x="-5.75" y="0.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="54" x="-5.75" y="1.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="53" x="-5.75" y="1.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="52" x="-5.75" y="2.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="51" x="-5.75" y="2.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="50" x="-5.75" y="3.25" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<smd name="49" x="-5.75" y="3.75" dx="1.2" dy="0.3" layer="1" rot="R180"/>
<wire x1="-5" y1="5" x2="5" y2="5" width="0.127" layer="21"/>
<wire x1="5" y1="5" x2="5" y2="-5" width="0.127" layer="21"/>
<wire x1="5" y1="-5" x2="-5" y2="-5" width="0.127" layer="21"/>
<wire x1="-5" y1="-5" x2="-5" y2="5" width="0.127" layer="21"/>
<wire x1="-6.25" y1="5.25" x2="-6.25" y2="6.25" width="0.127" layer="21"/>
<wire x1="-6.25" y1="6.25" x2="-5.25" y2="6.25" width="0.127" layer="21"/>
<wire x1="5.25" y1="6.25" x2="6.25" y2="6.25" width="0.127" layer="21"/>
<wire x1="6.25" y1="6.25" x2="6.25" y2="5.25" width="0.127" layer="21"/>
<wire x1="6.25" y1="-5.25" x2="6.25" y2="-6.25" width="0.127" layer="21"/>
<wire x1="6.25" y1="-6.25" x2="5.25" y2="-6.25" width="0.127" layer="21"/>
<wire x1="-5.25" y1="-6.25" x2="-6.25" y2="-6.25" width="0.127" layer="21"/>
<wire x1="-6.25" y1="-6.25" x2="-6.25" y2="-5.25" width="0.127" layer="21"/>
<circle x="-5.75" y="-5.75" radius="0.353553125" width="0.127" layer="21"/>
<text x="-6.25" y="6.5" size="1.27" layer="25">&gt;NAME</text>
<text x="-6" y="-8" size="1.27" layer="27">&gt;Value</text>
</package>
</packages>
<symbols>
<symbol name="STM32G071">
<pin name="PC11" x="-38.1" y="39.37" length="middle"/>
<pin name="PC12" x="-38.1" y="36.83" length="middle"/>
<pin name="PC13" x="-38.1" y="34.29" length="middle"/>
<pin name="P14-OSC32_IN" x="-38.1" y="31.75" length="middle"/>
<pin name="PC15-OSC32_OUT" x="-38.1" y="29.21" length="middle"/>
<pin name="VBAT" x="-38.1" y="26.67" length="middle"/>
<pin name="VREF+" x="-38.1" y="24.13" length="middle"/>
<pin name="VDD/VDDA" x="-38.1" y="21.59" length="middle"/>
<pin name="VSS/VSSA" x="-38.1" y="19.05" length="middle"/>
<pin name="PF0-OSC_IN" x="-38.1" y="16.51" length="middle"/>
<pin name="PF1-OSC_OUT" x="-38.1" y="13.97" length="middle"/>
<pin name="PF2-NRST" x="-38.1" y="11.43" length="middle"/>
<pin name="PC0" x="-38.1" y="8.89" length="middle"/>
<pin name="PC1" x="-38.1" y="6.35" length="middle"/>
<pin name="PC2" x="-38.1" y="3.81" length="middle"/>
<pin name="PC3" x="-38.1" y="1.27" length="middle"/>
<pin name="PA0" x="-38.1" y="-1.27" length="middle"/>
<pin name="PA1" x="-38.1" y="-3.81" length="middle"/>
<pin name="PA2" x="-38.1" y="-6.35" length="middle"/>
<pin name="PA3" x="-38.1" y="-8.89" length="middle"/>
<pin name="PA4" x="-38.1" y="-11.43" length="middle"/>
<pin name="PA5" x="-38.1" y="-13.97" length="middle"/>
<pin name="PA6" x="-38.1" y="-16.51" length="middle"/>
<pin name="PA7" x="-38.1" y="-19.05" length="middle"/>
<pin name="PC4" x="-38.1" y="-21.59" length="middle"/>
<pin name="PC5" x="-38.1" y="-24.13" length="middle"/>
<pin name="PB0" x="-38.1" y="-26.67" length="middle"/>
<pin name="PB1" x="-38.1" y="-29.21" length="middle"/>
<pin name="PB2" x="-38.1" y="-31.75" length="middle"/>
<pin name="PB10" x="-38.1" y="-34.29" length="middle"/>
<pin name="PB11" x="-38.1" y="-36.83" length="middle"/>
<pin name="PB12" x="-38.1" y="-39.37" length="middle"/>
<pin name="PB13" x="38.1" y="39.37" length="middle" rot="R180"/>
<pin name="PB14" x="38.1" y="36.83" length="middle" rot="R180"/>
<pin name="PB15" x="38.1" y="34.29" length="middle" rot="R180"/>
<pin name="PA8" x="38.1" y="31.75" length="middle" rot="R180"/>
<pin name="PA9" x="38.1" y="29.21" length="middle" rot="R180"/>
<pin name="PC6" x="38.1" y="26.67" length="middle" rot="R180"/>
<pin name="PC7" x="38.1" y="24.13" length="middle" rot="R180"/>
<pin name="PD8" x="38.1" y="21.59" length="middle" rot="R180"/>
<pin name="PD9" x="38.1" y="19.05" length="middle" rot="R180"/>
<pin name="PA10" x="38.1" y="16.51" length="middle" rot="R180"/>
<pin name="PA11[PA9]" x="38.1" y="13.97" length="middle" rot="R180"/>
<pin name="PA12[PA10]" x="38.1" y="11.43" length="middle" rot="R180"/>
<pin name="PA13" x="38.1" y="8.89" length="middle" rot="R180"/>
<pin name="PA14-BOOT0" x="38.1" y="6.35" length="middle" rot="R180"/>
<pin name="PA15" x="38.1" y="3.81" length="middle" rot="R180"/>
<pin name="PC8" x="38.1" y="1.27" length="middle" rot="R180"/>
<pin name="PC9" x="38.1" y="-1.27" length="middle" rot="R180"/>
<pin name="PD0" x="38.1" y="-3.81" length="middle" rot="R180"/>
<pin name="PD1" x="38.1" y="-6.35" length="middle" rot="R180"/>
<pin name="PD2" x="38.1" y="-8.89" length="middle" rot="R180"/>
<pin name="PD3" x="38.1" y="-11.43" length="middle" rot="R180"/>
<pin name="PD4" x="38.1" y="-13.97" length="middle" rot="R180"/>
<pin name="PD5" x="38.1" y="-16.51" length="middle" rot="R180"/>
<pin name="PD6" x="38.1" y="-19.05" length="middle" rot="R180"/>
<pin name="PB3" x="38.1" y="-21.59" length="middle" rot="R180"/>
<pin name="PB4" x="38.1" y="-24.13" length="middle" rot="R180"/>
<pin name="PB5" x="38.1" y="-26.67" length="middle" rot="R180"/>
<pin name="PB6" x="38.1" y="-29.21" length="middle" rot="R180"/>
<pin name="PB7" x="38.1" y="-31.75" length="middle" rot="R180"/>
<pin name="PB8" x="38.1" y="-34.29" length="middle" rot="R180"/>
<pin name="PB9" x="38.1" y="-36.83" length="middle" rot="R180"/>
<pin name="PC10" x="38.1" y="-39.37" length="middle" rot="R180"/>
<wire x1="-33.02" y1="43.18" x2="33.02" y2="43.18" width="0.254" layer="94"/>
<wire x1="33.02" y1="43.18" x2="33.02" y2="-44.45" width="0.254" layer="94"/>
<wire x1="33.02" y1="-44.45" x2="-33.02" y2="-44.45" width="0.254" layer="94"/>
<wire x1="-33.02" y1="-44.45" x2="-33.02" y2="43.18" width="0.254" layer="94"/>
<text x="-31.75" y="44.45" size="1.27" layer="95">&gt;NAME</text>
<text x="-31.75" y="-46.99" size="1.27" layer="96">&gt;VALUE</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="STM32GO71">
<gates>
<gate name="G$1" symbol="STM32G071" x="0" y="0"/>
</gates>
<devices>
<device name="" package="LQFP64">
<connects>
<connect gate="G$1" pin="P14-OSC32_IN" pad="4"/>
<connect gate="G$1" pin="PA0" pad="17"/>
<connect gate="G$1" pin="PA1" pad="18"/>
<connect gate="G$1" pin="PA10" pad="42"/>
<connect gate="G$1" pin="PA11[PA9]" pad="43"/>
<connect gate="G$1" pin="PA12[PA10]" pad="44"/>
<connect gate="G$1" pin="PA13" pad="45"/>
<connect gate="G$1" pin="PA14-BOOT0" pad="46"/>
<connect gate="G$1" pin="PA15" pad="47"/>
<connect gate="G$1" pin="PA2" pad="19"/>
<connect gate="G$1" pin="PA3" pad="20"/>
<connect gate="G$1" pin="PA4" pad="21"/>
<connect gate="G$1" pin="PA5" pad="22"/>
<connect gate="G$1" pin="PA6" pad="23"/>
<connect gate="G$1" pin="PA7" pad="24"/>
<connect gate="G$1" pin="PA8" pad="36"/>
<connect gate="G$1" pin="PA9" pad="37"/>
<connect gate="G$1" pin="PB0" pad="27"/>
<connect gate="G$1" pin="PB1" pad="28"/>
<connect gate="G$1" pin="PB10" pad="30"/>
<connect gate="G$1" pin="PB11" pad="31"/>
<connect gate="G$1" pin="PB12" pad="32"/>
<connect gate="G$1" pin="PB13" pad="33"/>
<connect gate="G$1" pin="PB14" pad="34"/>
<connect gate="G$1" pin="PB15" pad="35"/>
<connect gate="G$1" pin="PB2" pad="29"/>
<connect gate="G$1" pin="PB3" pad="57"/>
<connect gate="G$1" pin="PB4" pad="58"/>
<connect gate="G$1" pin="PB5" pad="59"/>
<connect gate="G$1" pin="PB6" pad="60"/>
<connect gate="G$1" pin="PB7" pad="61"/>
<connect gate="G$1" pin="PB8" pad="62"/>
<connect gate="G$1" pin="PB9" pad="63"/>
<connect gate="G$1" pin="PC0" pad="13"/>
<connect gate="G$1" pin="PC1" pad="14"/>
<connect gate="G$1" pin="PC10" pad="64"/>
<connect gate="G$1" pin="PC11" pad="1"/>
<connect gate="G$1" pin="PC12" pad="2"/>
<connect gate="G$1" pin="PC13" pad="3"/>
<connect gate="G$1" pin="PC15-OSC32_OUT" pad="5"/>
<connect gate="G$1" pin="PC2" pad="15"/>
<connect gate="G$1" pin="PC3" pad="16"/>
<connect gate="G$1" pin="PC4" pad="25"/>
<connect gate="G$1" pin="PC5" pad="26"/>
<connect gate="G$1" pin="PC6" pad="38"/>
<connect gate="G$1" pin="PC7" pad="39"/>
<connect gate="G$1" pin="PC8" pad="48"/>
<connect gate="G$1" pin="PC9" pad="49"/>
<connect gate="G$1" pin="PD0" pad="50"/>
<connect gate="G$1" pin="PD1" pad="51"/>
<connect gate="G$1" pin="PD2" pad="52"/>
<connect gate="G$1" pin="PD3" pad="53"/>
<connect gate="G$1" pin="PD4" pad="54"/>
<connect gate="G$1" pin="PD5" pad="55"/>
<connect gate="G$1" pin="PD6" pad="56"/>
<connect gate="G$1" pin="PD8" pad="40"/>
<connect gate="G$1" pin="PD9" pad="41"/>
<connect gate="G$1" pin="PF0-OSC_IN" pad="10"/>
<connect gate="G$1" pin="PF1-OSC_OUT" pad="11"/>
<connect gate="G$1" pin="PF2-NRST" pad="12"/>
<connect gate="G$1" pin="VBAT" pad="6"/>
<connect gate="G$1" pin="VDD/VDDA" pad="8"/>
<connect gate="G$1" pin="VREF+" pad="7"/>
<connect gate="G$1" pin="VSS/VSSA" pad="9"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="frames" urn="urn:adsk.eagle:library:229">
<description>&lt;b&gt;Frames for Sheet and Layout&lt;/b&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="FRAME_B_L" urn="urn:adsk.eagle:symbol:13883/1" library_version="1">
<frame x1="0" y1="0" x2="431.8" y2="279.4" columns="9" rows="6" layer="94" border-bottom="no"/>
</symbol>
<symbol name="DOCFIELD" urn="urn:adsk.eagle:symbol:13864/1" library_version="1">
<wire x1="0" y1="0" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="87.63" y2="15.24" width="0.1016" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="71.12" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="0" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="87.63" y1="5.08" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="101.6" y1="5.08" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="0" y1="15.24" x2="0" y2="22.86" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="22.86" x2="101.6" y2="15.24" width="0.1016" layer="94"/>
<text x="1.27" y="1.27" size="2.54" layer="94">Date:</text>
<text x="12.7" y="1.27" size="2.54" layer="94">&gt;LAST_DATE_TIME</text>
<text x="72.39" y="1.27" size="2.54" layer="94">Sheet:</text>
<text x="86.36" y="1.27" size="2.54" layer="94">&gt;SHEET</text>
<text x="88.9" y="11.43" size="2.54" layer="94">REV:</text>
<text x="1.27" y="19.05" size="2.54" layer="94">TITLE:</text>
<text x="1.27" y="11.43" size="2.54" layer="94">Document Number:</text>
<text x="17.78" y="19.05" size="2.54" layer="94">&gt;DRAWING_NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="FRAME_B_L" urn="urn:adsk.eagle:component:13938/1" prefix="FRAME" uservalue="yes" library_version="1">
<description>&lt;b&gt;FRAME&lt;/b&gt; B Size , 11 x 17 INCH, Landscape&lt;p&gt;</description>
<gates>
<gate name="G$1" symbol="FRAME_B_L" x="0" y="0" addlevel="always"/>
<gate name="G$2" symbol="DOCFIELD" x="325.12" y="0" addlevel="always"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="TSAR_Controller" deviceset="STM32GO71" device=""/>
<part name="FRAME1" library="frames" library_urn="urn:adsk.eagle:library:229" deviceset="FRAME_B_L" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="414.02" y="7.62" size="2.54" layer="94">V1.0</text>
<text x="363.22" y="10.16" size="2.54" layer="94">TSAR 0.0.0.1</text>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="182.88" y="160.02" smashed="yes">
<attribute name="NAME" x="151.13" y="204.47" size="1.27" layer="95"/>
<attribute name="VALUE" x="151.13" y="113.03" size="1.27" layer="96"/>
</instance>
<instance part="FRAME1" gate="G$1" x="0" y="0" smashed="yes"/>
<instance part="FRAME1" gate="G$2" x="325.12" y="0" smashed="yes">
<attribute name="LAST_DATE_TIME" x="337.82" y="1.27" size="2.54" layer="94"/>
<attribute name="SHEET" x="411.48" y="1.27" size="2.54" layer="94"/>
<attribute name="DRAWING_NAME" x="342.9" y="19.05" size="2.54" layer="94"/>
</instance>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
