
/**
 * @brief A base class for clonable objects.
 *
 * This class provides a virtual clone method that derived classes must implement.
 * It is used to create copies of objects in a polymorphic way.
 */
class Clonable
{
public:
    virtual ~Clonable() = default;
    virtual Clonable *clone() const = 0;
};


/**
 * @brief A reference-counted base class.
 *
 * This class implements reference counting to manage the lifetime of objects.
 */
class Ref
{
public:
    void retain();
    void release();
    Ref *autoRelease();
    unsigned int getReferenceCount() const;

protected:
    Ref();

public:
    virtual ~Ref();

protected:
    unsigned int referenceCount;
};
