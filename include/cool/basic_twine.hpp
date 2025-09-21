#ifndef INCLUDE_COOL_BASIC_TWINE_HPP
#define INCLUDE_COOL_BASIC_TWINE_HPP

#include <cassert>
#include <string>
#include <string_view>
#include <variant>

namespace cool {

template <typename CharT, typename Traits = std::char_traits<CharT>>
class BasicTwine final {
    using string = std::basic_string<CharT, Traits>;
    using string_view = std::basic_string_view<CharT, Traits>;

    using child_type = std::variant<std::monostate, string_view, const BasicTwine *>;

  public:
    BasicTwine() = default;

    explicit BasicTwine(string_view str) noexcept : left_{get_child(str)} {}

    BasicTwine(const BasicTwine &) = delete;
    BasicTwine &operator=(const BasicTwine &) = delete;

    BasicTwine(BasicTwine &&) = default;
    BasicTwine &operator=(BasicTwine &&) = default;

    ~BasicTwine() = default;

    bool empty() const noexcept {
        return std::holds_alternative<std::monostate>(left_) &&
               std::holds_alternative<std::monostate>(right_);
    }

    string str() const {
        if (std::holds_alternative<std::monostate>(left_)) {
            if (std::holds_alternative<std::monostate>(right_)) {
                return {};
            } else {
                assert(std::holds_alternative<string_view>(right_));
                return string{std::get<string_view>(right_)};
            }
        } else if (std::holds_alternative<std::monostate>(right_)) {
            assert(std::holds_alternative<string_view>(left_));
            return string{std::get<string_view>(left_)};
        }

        string str;
        to_string(str);
        return str;
    }

    friend BasicTwine operator+(BasicTwine &&lhs, BasicTwine &&rhs) noexcept {
        return BasicTwine{std::move(lhs), std::move(rhs)};
    }

    friend BasicTwine operator+(BasicTwine &&lhs, string_view rhs) noexcept {
        return BasicTwine{std::move(lhs), rhs};
    }

    friend BasicTwine operator+(string_view lhs, BasicTwine &&rhs) noexcept {
        return BasicTwine{lhs, std::move(rhs)};
    }

  private:
    BasicTwine(BasicTwine &&lhs, string_view rhs) noexcept
        : left_{get_child(std::move(lhs))}, right_{get_child(rhs)} {}

    BasicTwine(string_view lhs, BasicTwine &&rhs) noexcept
        : left_{get_child(lhs)}, right_{get_child(std::move(rhs))} {}

    BasicTwine(BasicTwine &&lhs, BasicTwine &&rhs) noexcept
        : left_{get_child(std::move(lhs))}, right_{get_child(std::move(rhs))} {}

    static child_type get_child(BasicTwine &&twine) noexcept {
        if (std::holds_alternative<std::monostate>(twine.left_)) {
            if (std::holds_alternative<std::monostate>(twine.right_)) {
                return {std::monostate{}};
            } else {
                return std::move(twine.right_);
            }
        } else if (std::holds_alternative<std::monostate>(twine.right_)) {
            return std::move(twine.left_);
        } else {
            return {&twine};
        }
    }

    static child_type get_child(string_view sv) noexcept {
        if (sv.empty()) {
            return {std::monostate{}};
        }
        return {sv};
    }

    void to_string(string &str) const {
        class Visitor final {
          public:
            explicit Visitor(string &str) noexcept : buffer(str) {}

            void operator()([[maybe_unused]] std::monostate) noexcept { /* no-op */ }
            void operator()(string_view str) { buffer.append(str); }
            void operator()(const BasicTwine *twine) { twine->to_string(buffer); }

          private:
            string &buffer;
        };

        Visitor visitor{str};
        std::visit(visitor, left_);
        std::visit(visitor, right_);
    }

    child_type left_;
    child_type right_;
};

} // end namespace cool

#endif // INCLUDE_COOL_BASIC_TWINE_HPP
