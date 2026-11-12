const TABLE_SIZE: usize = 100;

#[derive(Clone)]
struct CuckooEntry {
    key: String,
    value: String,
    is_occupied: bool,
}

pub struct CuckooTable {
    table1: [CuckooEntry; TABLE_SIZE],
    table2: [CuckooEntry; TABLE_SIZE],
    size: usize,
}

impl Default for CuckooTable {
    fn default() -> Self { Self::new() }
}

impl CuckooTable {
    pub fn new() -> Self {
        let empty = CuckooEntry { key: String::new(), value: String::new(), is_occupied: false };
        CuckooTable {
            table1: std::array::from_fn(|_| empty.clone()),
            table2: std::array::from_fn(|_| empty.clone()),
            size: 0,
        }
    }

    fn hash1(&self, key: &str) -> usize {
        let mut h = 0;
        for c in key.chars() { h = (h * 31 + c as usize) % TABLE_SIZE; }
        h
    }

    fn hash2(&self, key: &str) -> usize {
        let mut h = 5381;
        for c in key.chars() { h = ((h << 5) + h) + c as usize; }
        h % TABLE_SIZE
    }

    fn insert_helper(&mut self, key: &str, value: &str) -> bool {
        let mut cur_key = key.to_string();
        let mut cur_val = value.to_string();
        let mut use_t1 = true;

        for _ in 0..50 {
            if use_t1 {
                let idx = self.hash1(&cur_key);
                if !self.table1[idx].is_occupied {
                    self.table1[idx] = CuckooEntry { key: cur_key, value: cur_val, is_occupied: true };
                    return true;
                }
                let kicked_k = self.table1[idx].key.clone();
                let kicked_v = self.table1[idx].value.clone();
                self.table1[idx] = CuckooEntry { key: cur_key, value: cur_val, is_occupied: true };
                cur_key = kicked_k; cur_val = kicked_v;
                use_t1 = false;
            } else {
                let idx = self.hash2(&cur_key);
                if !self.table2[idx].is_occupied {
                    self.table2[idx] = CuckooEntry { key: cur_key, value: cur_val, is_occupied: true };
                    return true;
                }
                let kicked_k = self.table2[idx].key.clone();
                let kicked_v = self.table2[idx].value.clone();
                self.table2[idx] = CuckooEntry { key: cur_key, value: cur_val, is_occupied: true };
                cur_key = kicked_k; cur_val = kicked_v;
                use_t1 = true;
            }
        }
        false
    }

    pub fn add(&mut self, key: &str, value: &str) {
        if self.get(key).is_some() {
            self.del(key);
        }
        if self.insert_helper(key, value) {
            self.size += 1;
        }
    }

    pub fn get(&self, key: &str) -> Option<&String> {
        let idx1 = self.hash1(key);
        if self.table1[idx1].is_occupied && self.table1[idx1].key == key {
            return Some(&self.table1[idx1].value);
        }
        let idx2 = self.hash2(key);
        if self.table2[idx2].is_occupied && self.table2[idx2].key == key {
            return Some(&self.table2[idx2].value);
        }
        None
    }

    pub fn del(&mut self, key: &str) -> bool {
        let idx1 = self.hash1(key);
        if self.table1[idx1].is_occupied && self.table1[idx1].key == key {
            self.table1[idx1].is_occupied = false;
            self.size -= 1;
            return true;
        }
        let idx2 = self.hash2(key);
        if self.table2[idx2].is_occupied && self.table2[idx2].key == key {
            self.table2[idx2].is_occupied = false;
            self.size -= 1;
            return true;
        }
        false
    }

    pub fn len(&self) -> usize { self.size }
    pub fn is_empty(&self) -> bool { self.size == 0 }

    pub fn save_to_file(&self, filename: &str) {
        use std::fs::File;
        use std::io::Write;
        let mut file = File::create(filename).unwrap();
        for i in 0..TABLE_SIZE {
            if self.table1[i].is_occupied { writeln!(file, "{}:{}", self.table1[i].key, self.table1[i].value).unwrap(); }
            if self.table2[i].is_occupied { writeln!(file, "{}:{}", self.table2[i].key, self.table2[i].value).unwrap(); }
        }
    }

    pub fn load_from_file(&mut self, filename: &str) {
        use std::fs;
        *self = Self::new();
        if let Ok(content) = fs::read_to_string(filename) {
            for line in content.lines() {
                if let Some((k, v)) = line.split_once(':') { self.add(k, v); }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_cuckoo_full() {
        let mut ct = CuckooTable::new();
        assert!(ct.is_empty());

        ct.add("apple", "10");
        ct.add("banana", "20");
        assert_eq!(ct.get("apple").unwrap(), "10");
        assert_eq!(ct.get("banana").unwrap(), "20");

        ct.add("apple", "99");
        assert_eq!(ct.get("apple").unwrap(), "99");
        assert_eq!(ct.len(), 2);

        assert!(ct.del("apple"));
        assert!(!ct.del("none"));
        assert_eq!(ct.get("apple"), None);

        ct.save_to_file("test_cuckoo.txt");
        let mut ct2 = CuckooTable::new();
        ct2.load_from_file("test_cuckoo.txt");
        assert_eq!(ct2.get("banana").unwrap(), "20");
    }
}