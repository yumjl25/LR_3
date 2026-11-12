use std::rc::Rc;
use std::cell::RefCell;

pub type Link = Option<Rc<RefCell<TNode>>>;

pub struct TNode {
    pub data: i32,
    pub height: i32,
    pub left: Link,
    pub right: Link,
}

pub struct AVLTree {
    root: Link,
    size: usize,
}

impl Default for AVLTree {
    fn default() -> Self { Self::new() }
}

impl AVLTree {
    pub fn new() -> Self { AVLTree { root: None, size: 0 } }

    fn height(node: &Link) -> i32 { node.as_ref().map_or(0, |n| n.borrow().height) }

    fn update_height(node: &Link) {
        if let Some(n) = node {
            let hl = Self::height(&n.borrow().left);
            let hr = Self::height(&n.borrow().right);
            n.borrow_mut().height = 1 + hl.max(hr);
        }
    }

    fn balance_factor(node: &Link) -> i32 {
        node.as_ref().map_or(0, |n| Self::height(&n.borrow().left) - Self::height(&n.borrow().right))
    }

    fn rotate_right(y: Link) -> Link {
        let y_rc = y.unwrap();
        let x = y_rc.borrow_mut().left.take().unwrap();
        let b = x.borrow_mut().right.take();
        y_rc.borrow_mut().left = b;
        x.borrow_mut().right = Some(Rc::clone(&y_rc));
        Self::update_height(&Some(Rc::clone(&y_rc)));
        Self::update_height(&Some(Rc::clone(&x)));
        Some(x)
    }

    fn rotate_left(x: Link) -> Link {
        let x_rc = x.unwrap();
        let y = x_rc.borrow_mut().right.take().unwrap();
        let b = y.borrow_mut().left.take();
        x_rc.borrow_mut().right = b;
        y.borrow_mut().left = Some(Rc::clone(&x_rc));
        Self::update_height(&Some(Rc::clone(&x_rc)));
        Self::update_height(&Some(Rc::clone(&y)));
        Some(y)
    }

    fn balance(node: Link) -> Link {
        node.as_ref()?;
        Self::update_height(&node);
        let bf = Self::balance_factor(&node);

        if bf > 1 {
            if Self::balance_factor(&node.as_ref().unwrap().borrow().left) < 0 {
                let left = node.as_ref().unwrap().borrow_mut().left.take();
                node.as_ref().unwrap().borrow_mut().left = Self::rotate_left(left);
            }
            return Self::rotate_right(node);
        }
        if bf < -1 {
            if Self::balance_factor(&node.as_ref().unwrap().borrow().right) > 0 {
                let right = node.as_ref().unwrap().borrow_mut().right.take();
                node.as_ref().unwrap().borrow_mut().right = Self::rotate_right(right);
            }
            return Self::rotate_left(node);
        }
        node
    }

    pub fn insert(&mut self, val: i32) {
        let mut added = false;
        self.root = Self::insert_rec(self.root.take(), val, &mut added);
        if added { self.size += 1; }
    }

    fn insert_rec(node: Link, val: i32, added: &mut bool) -> Link {
        if node.is_none() {
            *added = true;
            return Some(Rc::new(RefCell::new(TNode { data: val, height: 1, left: None, right: None })));
        }
        let n = node.unwrap();
        if val < n.borrow().data {
            let left = n.borrow_mut().left.take();
            n.borrow_mut().left = Self::insert_rec(left, val, added);
        } else if val > n.borrow().data {
            let right = n.borrow_mut().right.take();
            n.borrow_mut().right = Self::insert_rec(right, val, added);
        } else {
            return Some(n); 
        }
        Self::balance(Some(n))
    }

    pub fn search(&self, val: i32) -> bool {
        let mut cur = self.root.clone();
        while let Some(node) = cur {
            let borrowed = node.borrow();
            if borrowed.data == val { return true; }
            if val < borrowed.data { cur = borrowed.left.clone(); }
            else { cur = borrowed.right.clone(); }
        }
        false
    }

    // --- Удаление ---
    fn find_min(node: &Link) -> Link {
        let mut cur = node.clone();
        while let Some(n) = cur {
            let next = n.borrow().left.clone();
            if next.is_none() { return Some(n); }
            cur = next;
        }
        None
    }

    pub fn del(&mut self, val: i32) -> bool {
        let mut deleted = false;
        self.root = Self::del_rec(self.root.take(), val, &mut deleted);
        if deleted { self.size -= 1; }
        deleted
    }

    fn del_rec(node: Link, val: i32, deleted: &mut bool) -> Link {
        node.as_ref()?;
        let n = node.unwrap();

        if val < n.borrow().data {
            let left = n.borrow_mut().left.take();
            n.borrow_mut().left = Self::del_rec(left, val, deleted);
        } else if val > n.borrow().data {
            let right = n.borrow_mut().right.take();
            n.borrow_mut().right = Self::del_rec(right, val, deleted);
        } else {
            *deleted = true;
            let has_left = n.borrow().left.is_some();
            let has_right = n.borrow().right.is_some();

            if !has_left && !has_right { return None; }
            else if !has_left { return n.borrow().right.clone(); }
            else if !has_right { return n.borrow().left.clone(); }
            else {
                let right = n.borrow().right.clone();
                let min_node = Self::find_min(&right).unwrap();
                let min_val = min_node.borrow().data;
                n.borrow_mut().data = min_val;
                let new_right = n.borrow_mut().right.take();
                let mut dummy_deleted = false;
                n.borrow_mut().right = Self::del_rec(new_right, min_val, &mut dummy_deleted);
            }
        }
        Self::balance(Some(n))
    }

    pub fn len(&self) -> usize { self.size }
    pub fn is_empty(&self) -> bool { self.size == 0 }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        let mut file = File::create(filename).unwrap();
        self.save_node(&self.root, &mut file);
    }

    fn save_node(&self, node: &Link, file: &mut std::fs::File) {
        use std::io::Write;
        if let Some(n) = node {
            let borrowed = n.borrow();
            self.save_node(&borrowed.left, file);
            writeln!(file, "{}", borrowed.data).unwrap();
            self.save_node(&borrowed.right, file);
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        *self = Self::new();
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if let Ok(val) = line.parse::<i32>() { self.insert(val); }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_avl_full() {
        let mut tree = AVLTree::new();
        assert!(tree.is_empty());

        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.insert(2);
        tree.insert(7);
        assert_eq!(tree.len(), 5);
        assert!(tree.search(7));
        assert!(!tree.search(99));

        assert!(tree.del(2));
        assert!(!tree.search(2));

        assert!(tree.del(5));

        assert!(tree.del(10));
        assert!(!tree.search(10));
        assert_eq!(tree.len(), 2);

        assert!(!tree.del(999));

        tree.save_to_file("test_avl.txt");
        let mut tree2 = AVLTree::new();
        tree2.load_from_file("test_avl.txt");
        assert_eq!(tree2.len(), 2);
    }
}