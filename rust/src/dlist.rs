use std::rc::{Rc, Weak};
use std::cell::RefCell;

pub type StrongLink = Option<Rc<RefCell<Node>>>;
pub type WeakLink = Option<Weak<RefCell<Node>>>;

pub struct Node {
    pub data: String,
    pub prev: WeakLink,
    pub next: StrongLink,
}

pub struct List {
    head: StrongLink,
    tail: StrongLink,
    size: usize,
}

impl Default for List {
    fn default() -> Self { Self::new() }
}

impl List {
    pub fn new() -> Self { List { head: None, tail: None, size: 0 } }

    pub fn push_head(&mut self, val: &str) {
        let node = Rc::new(RefCell::new(Node { data: val.to_string(), prev: None, next: self.head.clone() }));
        match &self.head {
            Some(h) => h.borrow_mut().prev = Some(Rc::downgrade(&node)),
            None => self.tail = Some(Rc::clone(&node)),
        }
        self.head = Some(node);
        self.size += 1;
    }

    pub fn push_tail(&mut self, val: &str) {
        let node = Rc::new(RefCell::new(Node { data: val.to_string(), prev: self.tail.as_ref().map(Rc::downgrade), next: None }));
        match &self.tail {
            Some(t) => t.borrow_mut().next = Some(Rc::clone(&node)),
            None => self.head = Some(Rc::clone(&node)),
        }
        self.tail = Some(node);
        self.size += 1;
    }

    pub fn push_before(&mut self, target: &str, val: &str) {
        if let Some(head) = self.head.clone() {
            if head.borrow().data == target { self.push_head(val); return; }
        }
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == target {
                let prev = node.borrow().prev.clone();
                let new_node = Rc::new(RefCell::new(Node { data: val.to_string(), prev: prev.clone(), next: Some(Rc::clone(&node)) }));
                node.borrow_mut().prev = Some(Rc::downgrade(&new_node));
                match prev {
                    Some(weak) => { if let Some(prev_rc) = weak.upgrade() { prev_rc.borrow_mut().next = Some(Rc::clone(&new_node)); } }
                    None => { self.head = Some(Rc::clone(&new_node)); }
                }
                self.size += 1;
                return;
            }
            cur = node.borrow().next.clone();
        }
        println!("Target not found");
    }

    pub fn push_after(&mut self, target: &str, val: &str) {
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == target {
                let next = node.borrow().next.clone();
                let new_node = Rc::new(RefCell::new(Node { data: val.to_string(), prev: Some(Rc::downgrade(&node)), next: next.clone() }));
                node.borrow_mut().next = Some(Rc::clone(&new_node));
                match next {
                    Some(n) => { n.borrow_mut().prev = Some(Rc::downgrade(&new_node)); }
                    None => { self.tail = Some(Rc::clone(&new_node)); }
                }
                self.size += 1;
                return;
            }
            cur = node.borrow().next.clone();
        }
        println!("Target not found");
    }

    pub fn del_head(&mut self) -> Option<String> {
        self.head.take().map(|head| {
            let val = head.borrow().data.clone();
            let next = head.borrow().next.clone();
            match &next {
                Some(n) => n.borrow_mut().prev = None,
                None => self.tail = None,
            }
            self.head = next;
            self.size -= 1;
            val
        })
    }

    pub fn del_tail(&mut self) -> Option<String> {
        self.tail.take().map(|tail| {
            let val = tail.borrow().data.clone();
            let prev = tail.borrow().prev.clone();
            match prev {
                Some(weak) => {
                    if let Some(prev_rc) = weak.upgrade() {
                        prev_rc.borrow_mut().next = None;
                        self.tail = Some(Rc::clone(&prev_rc));
                    }
                }
                None => self.head = None,
            }
            self.size -= 1;
            val
        })
    }

    pub fn del_value(&mut self, target: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == target {
                let prev = node.borrow().prev.clone();
                let next = node.borrow().next.clone();
                match &prev {
                    Some(weak) => { if let Some(p) = weak.upgrade() { p.borrow_mut().next = next.clone(); } }
                    None => self.head = next.clone(),
                }
                match &next {
                    Some(n) => n.borrow_mut().prev = prev.clone(),
                    None => {
                        match prev {
                            Some(weak) => { if let Some(p) = weak.upgrade() { self.tail = Some(p); } }
                            None => self.tail = None,
                        }
                    }
                }
                self.size -= 1;
                return true;
            }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn search(&self, target: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            if node.borrow().data == target { return true; }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn get_head(&self) -> Option<String> { self.head.as_ref().map(|n| n.borrow().data.clone()) }
    pub fn get_tail(&self) -> Option<String> { self.tail.as_ref().map(|n| n.borrow().data.clone()) }
    pub fn len(&self) -> usize { self.size }
    pub fn is_empty(&self) -> bool { self.size == 0 }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        use std::io::Write;
        let mut file = File::create(filename).unwrap();
        let mut cur = self.head.clone();
        while let Some(node) = cur {
            writeln!(file, "{}", node.borrow().data).unwrap();
            cur = node.borrow().next.clone();
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if !line.is_empty() { self.push_tail(line); }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_dlist_full() {
        let mut dl = List::new();
        assert!(dl.is_empty());

        dl.push_tail("B");
        dl.push_head("A"); // A, B
        dl.push_tail("D"); // A, B, D
        dl.push_before("D", "C"); // A, B, C, D
        dl.push_after("D", "E"); // A, B, C, D, E

        assert_eq!(dl.len(), 5);
        assert_eq!(dl.get_head().unwrap(), "A");
        assert_eq!(dl.get_tail().unwrap(), "E");
        assert!(dl.search("C"));

        assert_eq!(dl.del_head().unwrap(), "A");
        assert_eq!(dl.del_tail().unwrap(), "E");
        assert!(dl.del_value("C"));
        assert_eq!(dl.len(), 2);

        dl.push_before("none", "val");
        dl.push_after("none", "val");

        dl.save_to_file("test_dlist.txt");
        let mut dl2 = List::new();
        dl2.load_from_file("test_dlist.txt");
        assert_eq!(dl2.len(), 2);
    }
}