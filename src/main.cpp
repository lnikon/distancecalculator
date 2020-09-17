#include <iostream>
#include <optional>
#include <thread>
#include <vector>

enum class DistanceCalculatorEngineType
{
    Sequential = 0,
    CPPThreads,
    CUDA,
    MPI
};

template <typename ValueType>
class IDistanceCalculatorEngine
{
public:
    virtual std::vector<std::vector<ValueType>> calculate() /* noexcept */ const = 0;
    virtual DistanceCalculatorEngineType type() /* noexcept */ const = 0;
};

template <typename ValueType>
class SequentialDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    std::vector<std::vector<ValueType>> calculate() /* noexcept */ const override
    {
        return std::vector<std::vector<ValueType>>{};
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::Sequential;
    }
};

template <typename ValueType>
class CPPThreadsDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    std::vector<std::vector<ValueType>> calculate() /* noexcept */ const override
    {
        return std::vector<std::vector<ValueType>>{};
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::CPPThreads;
    }
};

template <typename ValueType>
class CUDADistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    std::vector<std::vector<ValueType>> calculate() /* noexcept */ const override
    {
        return std::vector<std::vector<ValueType>>{};
    }

    DistanceCalculatorEngineType type() const /* noexcept */ override
    {
        return DistanceCalculatorEngineType::CUDA;
    }
};

template <typename ValueType>
class MPIDistanceCalculatorEngine : public IDistanceCalculatorEngine<ValueType>
{
public:
    struct Options
    {
    };

    explicit MPIDistanceCalculatorEngine() = default;

    explicit MPIDistanceCalculatorEngine(const Options& options)
        : m_options(options)
    {
    }

    std::vector<std::vector<ValueType>> calculate() /* noexcept */ const override
    {
        return std::vector<std::vector<ValueType>>{};
    }

    DistanceCalculatorEngineType type() /* noexcept */ const override
    {
        return DistanceCalculatorEngineType::MPI;
    }

private:
    Options m_options;
};

template <typename ValueType>
std::shared_ptr<IDistanceCalculatorEngine<ValueType>>
distanceCalculatorEngineFactory(const DistanceCalculatorEngineType type)
{
    switch (type)
    {
    case DistanceCalculatorEngineType::Sequential:
        return std::make_shared<SequentialDistanceCalculatorEngine<ValueType>>();
    case DistanceCalculatorEngineType::CPPThreads:
        return std::make_shared<CPPThreadsDistanceCalculatorEngine<ValueType>>();
    case DistanceCalculatorEngineType::CUDA:
        return std::make_shared<CUDADistanceCalculatorEngine<ValueType>>();
    case DistanceCalculatorEngineType::MPI:
        return std::make_shared<MPIDistanceCalculatorEngine<ValueType>>();
    default:
        return nullptr;
    }
}

/*
 * Initially distance calculation will be possible only with one engine
 */
template <typename ValueType>
class DistanceCalculator
{
public:
    void setDistanceCalculatorEngineType(const DistanceCalculatorEngineType type)
    {
        if (auto engine = distanceCalculatorEngineFactory<ValueType>(type); engine != nullptr)
        {
            m_engine = engine;
        }
    }

private:
    std::shared_ptr<IDistanceCalculatorEngine<ValueType>> m_engine{nullptr};
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    DistanceCalculator<float> dc;
    dc.setDistanceCalculatorEngineType(DistanceCalculatorEngineType::CPPThreads);
    return 0;
}
