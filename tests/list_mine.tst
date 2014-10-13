list /
list //
list ///
create /test 123
create /test/a 456
create /test/b 789
list /test
list /test/a
list /test/b
list test
remove /test/b
list /test
list /missing_key
list /
remove /test/a
list /test
list /
