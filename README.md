# libresolver

## TODO

* Some sequences of instructions should be considered _atomic_
  in the tree. For example, the `ja` followed by a `cmp`.
* Check if different actions should be an error. It probably
  shouldn't. Search for "used by other pattern" in the cases.
* Does register size really matter? Do we need a pattern for
  byte, word, double-words and quad-words? Or a catch-all pattern
  is good enough?
* We use AT&T syntax. Make sure the code base follows that.
* The instruction `cltq` sign-extend %eax into %rax. Check
  if there is equivalent instructions for other register.
  If so, they should match the same instruction in the
  pattern tree.
* When rewinding instructions, we need to go through possible
  jump source. For example, some instructions of a pattern are
  match in a block A, then jump to a block B where the rest of
  the instructions are.
    * See `cmd_replace @ git/builtin/replace.c`.
    * What happens if there is multiple jump sources?