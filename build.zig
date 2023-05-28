const std = @import("std");

pub fn build(b: *std.build.Builder) !void {
    try build_main_exe(b);
    try build_test_exe(b);
}

fn build_main_exe(b: *std.build.Builder) !void {
    const chess_engine_exe = b.addExecutable(.{ .name = "chess_engine" });
    chess_engine_exe.addCSourceFiles(&.{
        "./src/BitBoard.cpp",
        "./src/BitBoardUtils.cpp",
        "./src/BitMaskValue.cpp",
        "./src/Board.cpp",
        "./src/CachedCapturesGeneration.cpp",
        "./src/CachedMoves.cpp",
        "./src/CapturesGeneration.cpp",
        "./src/Castle.cpp",
        "./src/Color.cpp",
        "./src/Definitions.cpp",
        "./src/FormatUtils.cpp",
        "./src/GameState.cpp",
        "./src/GameUtils.cpp",
        "./src/MagicBitBoards.cpp",
        "./src/Main.cpp",
        "./src/Move.cpp",
        "./src/Moves.cpp",
        "./src/MoveSearch.cpp",
        "./src/PieceCode.cpp",
        "./src/PieceValue.cpp",
        "./src/Position.cpp",
        "./src/SearchThread.cpp",
        "./src/SearchThreadPool.cpp",
        "./src/Square.cpp",
        "./src/StringUtils.cpp",
        "./src/TestFW.cpp",
        //"./src/TestMain.cpp",
        "./src/Timer.cpp",
        "./src/UCIUtils.cpp",
        "./src/ZobristHash.cpp",
    }, &.{ "-std=c++2b", "-W", "-Wall", "-Wextra", "-fexperimental-library", "-O3", "-static", "-flto", "-march=native" });
    chess_engine_exe.linkLibCpp();
    b.installArtifact(chess_engine_exe);

    const run_cmd = b.addRunArtifact(chess_engine_exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the engine");
    run_step.dependOn(&run_cmd.step);
}

fn build_test_exe(b: *std.Build.Builder) !void {
    const chess_engine_tests_exe = b.addExecutable(.{ .name = "chess_engine_tests" });
    chess_engine_tests_exe.addCSourceFiles(&.{
        "./src/BitBoard.cpp",
        "./src/BitBoardUtils.cpp",
        "./src/BitMaskValue.cpp",
        "./src/Board.cpp",
        "./src/CachedCapturesGeneration.cpp",
        "./src/CachedMoves.cpp",
        "./src/CapturesGeneration.cpp",
        "./src/Castle.cpp",
        "./src/Color.cpp",
        "./src/Definitions.cpp",
        "./src/FormatUtils.cpp",
        "./src/GameState.cpp",
        "./src/GameUtils.cpp",
        "./src/MagicBitBoards.cpp",
        //"./src/Main.cpp",
        "./src/Move.cpp",
        "./src/Moves.cpp",
        "./src/MoveSearch.cpp",
        "./src/PieceCode.cpp",
        "./src/PieceValue.cpp",
        "./src/Position.cpp",
        "./src/SearchThread.cpp",
        "./src/SearchThreadPool.cpp",
        "./src/Square.cpp",
        "./src/StringUtils.cpp",
        "./src/TestFW.cpp",
        "./src/TestMain.cpp",
        "./src/Timer.cpp",
        "./src/UCIUtils.cpp",
        "./src/ZobristHash.cpp",
        "./test/BoardTests.cpp",
        "./test/ConversionTests.cpp",
        "./test/GameStateTest.cpp",
        "./test/GenerateMoveTests.cpp",
        "./test/MagicBitBoardsTests.cpp",
        "./test/MoveSearchTests.cpp",
        "./test/MoveTests.cpp",
        "./test/ProcessMoveTests.cpp",
        "./test/ShiftTests.cpp",
        "./test/Tests.cpp",
    }, &.{ "-std=c++2b", "-W", "-Wall", "-Wextra", "-fexperimental-library", "-O3", "-static", "-flto", "-march=native" });
    chess_engine_tests_exe.linkLibCpp();
    b.installArtifact(chess_engine_tests_exe);

    const test_cmd = b.addRunArtifact(chess_engine_tests_exe);
    test_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        test_cmd.addArgs(args);
    }

    const test_step = b.step("test", "Test the engine");
    test_step.dependOn(&test_cmd.step);
}
