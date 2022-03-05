ocesql prog.cbl prog.cob
cobc -locesql -m prog.cob
LD_PRELOAD=/usr/local/lib/libocesql.so cobcrun prog