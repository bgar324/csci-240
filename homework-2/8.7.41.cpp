Position inorder_next(Position p) {
  if (!p.is_null()) {
      if (!p.right().is_null()) {
          // Go to the leftmost node in the right subtree
          Position walk = p.right();
          while (!walk.left().is_null())
              walk = walk.left();
          return walk;
      } else {
          // Move up until we come from the left child
          Position walk = p;
          while (!walk.is_root() && walk == walk.parent().right())
              walk = walk.parent();
          return walk.is_root() ? Position(nullptr) : walk.parent();
      }
  }
  return Position(nullptr);
}
