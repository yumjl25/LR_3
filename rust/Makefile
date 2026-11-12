
test:
	cargo test

lint:
	cargo clippy --all-targets -- -D warnings

coverage:
	CARGO_INCREMENTAL=0 RUSTFLAGS="-Cinstrument-coverage" LLVM_PROFILE_FILE="profile-%p-%m.profraw" cargo test
	grcov . --binary-path ./target/debug/deps/ -s . -t html --branch --ignore-not-existing -o ./coverage_report/

bench:
	cargo run --release --bin benchmark

clean:
	cargo clean
	rm -rf coverage_report *.profraw *.txt

.PHONY: test lint coverage bench clean