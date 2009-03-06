EXTRA_DIST = doxygen.cfg.in bdffile_interface.txt


html: doxygen.cfg
	doxygen $^

clean:
	$(RM) -r html/
