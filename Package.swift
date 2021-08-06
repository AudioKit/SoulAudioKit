// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "SoulAudioKit",
    platforms: [.macOS(.v10_13), .iOS(.v11), .tvOS(.v11)],
    products: [.library(name: "SoulAudioKit", targets: ["SoulAudioKit"])],
    dependencies: [.package(url: "https://github.com/AudioKit/AudioKit", from: "5.2.0")],
    targets: [
        .target(name: "SoulAudioKit", dependencies: ["AudioKit", "CSoulAudioKit"]),
        .target(name: "CSoulAudioKit", dependencies: ["AudioKit"]),
        .testTarget(name: "SoulAudioKitTests", dependencies: ["SoulAudioKit"], resources: [.copy("TestResources/")]),
    ],
    cxxLanguageStandard: .cxx14
)
    
