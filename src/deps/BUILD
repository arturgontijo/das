load("@rules_python//python:pip.bzl", "compile_pip_requirements")

package(default_visibility = ["//visibility:public"])

compile_pip_requirements(
    name = "requirements",
    src = "requirements.in",
    generate_hashes = False,
    requirements_txt = "requirements_lock.txt",
    visibility = ["//visibility:public"],
)

compile_pip_requirements(
    name = "requirements_dev",
    src = "requirements_dev.in",
    generate_hashes = False,
    requirements_txt = "requirements_dev_lock.txt",
    visibility = ["//visibility:public"],
)
