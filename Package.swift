// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SoulAudioKit",
    platforms: [.macOS(.v10_14), .iOS(.v13), .tvOS(.v13)],
    products: [.library(name: "SoulAudioKit", targets: ["SoulAudioKit"])],
    dependencies: [.package(url: "https://github.com/AudioKit/AudioKit", .branch("develop"))],
    targets: [
        .target(name: "SoulAudioKit", dependencies: ["AudioKit", "CSoulAudioKit"]),
        .target(name: "CSoulAudioKit", dependencies: ["AudioKit"]),
        .testTarget(name: "SoulAudioKitTests", dependencies: ["SoulAudioKit"], resources: [.copy("TestResources/")]),
    ],
    cxxLanguageStandard: .cxx14
)
    