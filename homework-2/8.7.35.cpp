int count_subtree(Node* nd) const {
  if (nd == nullptr) return 0;
  return 1 + count_subtree(nd->left) + count_subtree(nd->right);
}
//helper ^

void prune_subtree(Position p) {
  if (p.is_null()) return;

  Node* nd = p.node;
  int pruned_count = count_subtree(nd);

  if (nd == rt) {
      tear_down(rt);
      rt = nullptr;
  } else {
      Node* parent = nd->parent;
      if (parent->left == nd) parent->left = nullptr;
      else parent->right = nullptr;
      tear_down(nd);
  }

  sz -= pruned_count;
}
