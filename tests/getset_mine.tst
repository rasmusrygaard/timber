get /
get //
get foo
get foo/
get /foo/
set / 123
set // 123
set /// 123
set foo/ 123
set /foo/ 213
create /test 123
get /test
set /test foo
get /test
set /test bar
get /test
create /test/a abc
get /test/a
set /test/a b
get /test/a
remove /test/a
set /test/a 10
