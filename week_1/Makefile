DIRNAME = mdejonge_ibecht_assign1

all: midpoint triangle

midpoint:
	cd basic_midpoint; $(MAKE)

triangle:
	cd triangle_rasterization; $(MAKE)

clean:
	cd basic_midpoint; $(MAKE) clean
	cd triangle_rasterization; $(MAKE) clean

dist: clean
	@mkdir $(DIRNAME)
	@cp -r basic_midpoint triangle_rasterization $(DIRNAME)
	@tar -cf $(DIRNAME).tar $(DIRNAME)
	@gzip $(DIRNAME).tar
	@rm -r $(DIRNAME)

distclean:
	@rm $(DIRNAME).tar.gz
