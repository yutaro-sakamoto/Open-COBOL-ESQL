ocesql INSERTTBL.cbl INSERTTBL.cob
ocesql FETCHTBL.cbl FETCHTBL.cob
cobc -m -locesql INSERTTBL.cob
cobc -m -locesql FETCHTBL.cob
LD_PRELOAD=/usr/local/lib/libocesql.so cobcrun INSERTTBL
LD_PRELOAD=/usr/local/lib/libocesql.so cobcrun FETCHTBL
