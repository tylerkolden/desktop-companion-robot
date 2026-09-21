# Improve the teaching project

Keep changes small, reproducible, and understandable by a first-time learner. Discuss physical observations with the adult builder. Do not change supply wiring or motion limits silently.

1. Write the problem, one proposed change, and a pass condition.
2. Record a known baseline; use `git status` to see existing work.
3. Create a branch, for example `git switch -c lesson/clearer-eyes`.
4. Change one concept at a time. Explain it in the lesson or relevant document.
5. Compile changed sketches with `robot.ps1 build -Sketch NAME`; run `scripts/check.ps1` for toolchain/script changes. Run `python scripts/check_docs.py` for changed local links and mesh checks for changed CAD exports. Record actual physical tests separately.
6. Review `git diff`. Include source, instructions, and test evidence; exclude `.tools`, generated build output, credentials, and private notes.
7. Commit with a clear message, push your branch if authorized, and have another reader follow the instructions before merging.

Bug reports should include expected/actual behavior, sketch and commit, exact commands, connected parts, and the first error. Feature ideas belong in the plan with dependencies and acceptance criteria. A new feature is not complete until its setup and use are documented.

This is currently a private project without a project-wide redistribution license. Preserve upstream attribution. Do not import external CAD, images, or code without checking reuse terms. Publishing the repository is a separate owner decision.
# Wiring documentation requirement

Any wiring change needs a corresponding labelled diagram, pinout and connection table beside the instructions. Use the [wiring atlas](docs/wiring/README.md) and its reproducible generator. Identify exact hardware and distinguish functional maps from verified physical layouts. Regenerate the book when its wiring changes. Diagrams and compilation are not evidence of a physical test.
