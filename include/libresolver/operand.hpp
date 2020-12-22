#ifndef LIBRESOLVER_INCLUDE_OPERAND_HPP
#define LIBRESOLVER_INCLUDE_OPERAND_HPP

#include <memory>

#include <libresolver/placeholder.hpp>
#include <libresolver/utils/hash.hpp>
#include <libresolver/utils/registers.hpp>

namespace libresolver {

namespace operand {

class reg;
class imm;
class mem2;
class mem4;

class base {
   public:
    virtual ~base()                                  = default;
    virtual bool operator==(const base& other) const = 0;
    virtual bool operator==(const reg& other) const  = 0;
    virtual bool operator==(const imm& other) const  = 0;
    virtual bool operator==(const mem2& other) const = 0;
    virtual bool operator==(const mem4& other) const = 0;
    virtual size_t hash() const                      = 0;
    virtual std::string to_string() const            = 0;
};

class reg : public base {
   public:
    reg(placeholder::reg reg, utils::registers::size reg_size) : reg_(reg), reg_size_(reg_size) {}

    bool operator==(const base& other) const override {
        return (other == *this);
    }

    bool operator==(const reg& other) const override {
        return (reg_ == other.reg_) && (reg_size_ == other.reg_size_);
    }

    bool operator==(const imm& other) const override {
        return false;
    }

    bool operator==(const mem2& other) const override {
        return false;
    }

    bool operator==(const mem4& other) const override {
        return false;
    }

    size_t hash() const override {
        return utils::hash::combine(std::hash<placeholder::reg>{}(reg_), reg_size_);
    }

    std::string to_string() const override {
        return std::to_string(reg_) + std::to_string(reg_size_);
    }

   private:
    const placeholder::reg reg_;
    const utils::registers::size reg_size_;
};

class imm : public base {
   public:
    imm(placeholder::value value) : value_(value) {}

    bool operator==(const base& other) const override {
        return (other == *this);
    }

    bool operator==(const reg& other) const override {
        return false;
    }

    bool operator==(const imm& other) const override {
        return (value_ == other.value_);
    }

    bool operator==(const mem2& other) const override {
        return false;
    }

    bool operator==(const mem4& other) const override {
        return false;
    }

    size_t hash() const override {
        return std::hash<placeholder::value>{}(value_);
    }

    std::string to_string() const override {
        return std::to_string(value_);
    }

   private:
    const placeholder::value value_;
};

class mem2 : public base {
   public:
    mem2(placeholder::value disp, placeholder::reg base, utils::registers::size base_size)
        : base_(base), base_size_(base_size), disp_(disp) {}

    bool operator==(const base& other) const override {
        return (other == *this);
    }

    bool operator==(const reg& other) const override {
        return false;
    }

    bool operator==(const imm& other) const override {
        return false;
    }

    bool operator==(const mem2& other) const override {
        return (base_ == other.base_) && (base_size_ == other.base_size_) && (disp_ == other.disp_);
    }

    bool operator==(const mem4& other) const override {
        return false;
    }

    size_t hash() const override {
        return utils::hash::combine(std::hash<placeholder::reg>{}(base_), base_size_, disp_);
    }

    std::string to_string() const override {
        return std::to_string(disp_) + "(" + std::to_string(base_) + std::to_string(base_size_) + ")";
    }

   public:
    const placeholder::reg base_;
    const utils::registers::size base_size_;
    const placeholder::value disp_;
};

class mem4 : public base {
   public:
    mem4(placeholder::value disp, placeholder::reg base, utils::registers::size base_size, placeholder::reg index,
         utils::registers::size index_size, placeholder::value scale)
        : base_(base), base_size_(base_size), index_(index), index_size_(index_size), scale_(scale), disp_(disp) {}

    bool operator==(const base& other) const override {
        return (other == *this);
    }

    bool operator==(const reg& other) const override {
        return false;
    }

    bool operator==(const imm& other) const override {
        return false;
    }

    bool operator==(const mem2& other) const override {
        return false;
    }

    bool operator==(const mem4& other) const override {
        return (base_ == other.base_) && (base_size_ == other.base_size_) && (index_ == other.index_) &&
               (index_size_ == other.index_size_) && (scale_ == other.scale_) && (disp_ == other.disp_);
    }

    size_t hash() const override {
        return utils::hash::combine(std::hash<placeholder::reg>{}(base_), base_size_, index_, index_size_, scale_,
                                    disp_);
    }

    std::string to_string() const override {
        return std::to_string(disp_) + "(" + std::to_string(base_) + std::to_string(base_size_) + ", " +
               std::to_string(index_) + std::to_string(index_size_) + ", " + std::to_string(scale_) + ")";
    }

   public:
    const placeholder::reg base_;
    const utils::registers::size base_size_;
    const placeholder::reg index_;
    const utils::registers::size index_size_;
    const placeholder::value scale_;
    const placeholder::value disp_;
};

/*
 * We use polymorphism and polymorphism requires a pointer or a reference.
 * Rather than using raw pointer, we use std::shared_ptr<T>. The class
 * std::unique_ptr<T> would probably be better, but it is more heavy to
 * use (less implicit cast, etc.).
 */

using base_ptr = std::shared_ptr<base>;
using reg_ptr  = std::shared_ptr<reg>;
using imm_ptr  = std::shared_ptr<imm>;
using mem2_ptr = std::shared_ptr<mem2>;
using mem4_ptr = std::shared_ptr<mem4>;

template <typename... Args>
reg_ptr make_reg(Args... args) {
    return std::make_shared<reg>(args...);
}

template <typename... Args>
imm_ptr make_imm(Args... args) {
    return std::make_shared<imm>(args...);
}

template <typename... Args>
mem2_ptr make_mem2(Args... args) {
    return std::make_shared<mem2>(args...);
}

template <typename... Args>
mem4_ptr make_mem4(Args... args) {
    return std::make_shared<mem4>(args...);
}

} /* namespace operand */

} /* namespace libresolver */

namespace std {

template <>
struct hash<libresolver::operand::base> {
    size_t operator()(const libresolver::operand::base& operand) const {
        return operand.hash();
    }
};

template <>
struct hash<libresolver::operand::base_ptr> {
    size_t operator()(const libresolver::operand::base_ptr operand) const {
        return operand->hash();
    }
};

static inline string to_string(const libresolver::operand::base& op) {
    return op.to_string();
}

} /* namespace std */

#endif /* LIBRESOLVER_INCLUDE_OPERAND_HPP */