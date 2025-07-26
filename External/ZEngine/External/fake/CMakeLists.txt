project("fake")

add_library(fake OBJECT fake.cpp
        ../../Framework/Asset/UnknownAsset.h
        ../../Framework/Asset/UnknownAssetLoader.h)

target_sources(fake PRIVATE fake.cpp)
