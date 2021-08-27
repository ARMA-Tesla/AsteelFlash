dir *.c *.h /B >file.list
xgettext -f file.list -k_ -k_noop
