EXTRA_DIST = doxygen.cfg.in
dist_man_MANS = OpenLPTTrigger.3 CloseLPTTrigger.3 SignalTrigger.3	\
                OpenLPTPort.3 CloseLPTPort.3 SetLPTData.3		\
		lptpwm_open.3 lptpwm_close.3 lptpwm_setlevels.3		\
		testport.1 lpttrigger_get_string.3

html: doxygen.cfg
	doxygen $^

clean:
	$(RM) -r html/

