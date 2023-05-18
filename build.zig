const std = @import("std");

pub fn build(b: *std.build.Builder) !void {
    var sources = std.ArrayList([]const u8).init(b.allocator);

    // Search for all C/C++ files in `src` and add them
    {
        var dir = try std.fs.cwd().openIterableDir("src", .{});

        var walker = try dir.walk(b.allocator);
        defer walker.deinit();

        const allowed_exts = [_][]const u8{ ".c", ".cpp", ".cxx", ".c++", ".cc" };
        while (try walker.next()) |entry| {
            const ext = std.fs.path.extension(entry.basename);
            const include_file = for (allowed_exts) |e| {
                if (std.mem.eql(u8, ext, e))
                    break true;
            } else false;
            if (include_file) {
                // we have to clone the path as walker.next() or walker.deinit() will override/kill it
                try sources.append(entry.path);
            }
        }
    }

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
    }, &.{ "-std=c++2b", "-W", "-Wall", "-fexperimental-library", "-stdlib=libc++" });
    chess_engine_exe.linkLibCpp();
    b.installArtifact(chess_engine_exe);

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
    }, &.{ "-std=c++2b", "-W", "-Wall", "-fexperimental-library", "-stdlib=libc++" });
    chess_engine_tests_exe.linkLibCpp();
    b.installArtifact(chess_engine_tests_exe);
}
